#include "muduo/net/common/tcp_server.h"
#include "muduo/base/common/logging.h"
#include "muduo/base/define/define_new.h"
#include "muduo/net/common/acceptor.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/eventloop_thread_pool.h"
#include "muduo/net/common/sockets_ops.h"
#include <stdio.h>  // snprintf

using namespace muduo;
using namespace muduo::net;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr, const string& nameArg, Option option)
  : m_loop(CHECK_NOTNULL(loop))
  , m_ipPort(listenAddr.toIpPort())
  , m_name(nameArg)
  , m_acceptor(NEW Acceptor(loop, listenAddr, option == kReusePort))
  , m_threadPool(NEW EventLoopThreadPool(loop, m_name))
  , m_connectionCallback(defaultConnectionCallback)
  , m_messageCallback(defaultMessageCallback)
  , m_nextConnId(1)
{
  m_acceptor->setNewConnectionCallback(
                                       std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
  m_loop->AssertInLoopThread();
  LOG_TRACE << "TcpServer::~TcpServer [" << m_name << "] destructing";

  for (auto& item : m_connections)
  {
    TcpConnectionPtr conn(item.second);
    item.second.reset();
    conn->GetLoop()->RunInLoop(
                               std::bind(&TcpConnection::ConnectDestroyed, conn));
  }
}

void TcpServer::setThreadNum(int numThreads)
{
  assert(0 <= numThreads);
  m_threadPool->setThreadNum(numThreads);
}

void TcpServer::start()
{
  if (m_started.getAndSet(1) == 0)
  {
    m_threadPool->start(m_threadInitCallback);
    assert(!m_acceptor->IsListenning());
    m_loop->RunInLoop(
                      std::bind(&Acceptor::listen, get_pointer(m_acceptor)));
  }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
  m_loop->AssertInLoopThread();
  EventLoop* ioLoop = m_threadPool->getNextLoop();  // 从m_threadPool中取出一个线程给TcpConnection
  char buf[64];
  snprintf(buf, sizeof buf, "-%s#%d", m_ipPort.c_str(), m_nextConnId);
  ++m_nextConnId;
  string connName = m_name + buf;

  LOG_INFO << "TcpServer::newConnection [" << m_name << "] - new connection [" << connName << "] from " << peerAddr.toIpPort();
  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(NEW TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));  // 新建一个TcpConnection
  m_connections[connName] = conn;
  
  // 设置TcpConnection的回调函数
  conn->setConnectionCallback(m_connectionCallback);
  conn->setMessageCallback(m_messageCallback);
  conn->setWriteCompleteCallback(m_writeCompleteCallback);
  conn->setCloseCallback(
                         std::bind(&TcpServer::removeConnection, this, _1)); // FIXME: unsafe

  /*
   * TcpConnection::ConnectEstablished要放到它所属的那个事件驱动循环线程做，不要阻塞TcpServer线程。
   *    这个地方不是为了线程安全性考虑，因为TcpConnection本身就是在TcpServer线程创建的，暴露给TcpServer线程很正常，
   *    而且TcpServer中也记录着所有创建的TcpConnection，这里的主要目的是不阻塞TcpServer线程，让它继续监听客户端请求。
   * 所以需要调用TcpConnection的函数时，要放到它所在的线程中去调用 
   */
  ioLoop->RunInLoop(
                    std::bind(&TcpConnection::ConnectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	/*
   * FIXME: unsafe
	 * 在TcpConnection所在的事件驱动循环所在的线程执行删除工作，
	 * 因为需要操作TcpServer::connections_，就需要传TcpServer的this指针到TcpConnection所在线程，
	 * 会导致将TcpServer暴露给TcpConnection线程，也不具有线程安全性。
	 * 		调用这个函数的线程是TcpConnection所在线程，因为它被激活，然后调用回调函数，都是在自己线程执行的。
   * 		而removeConnection的调用者TcpServer的this指针如今在TcpConnection所在线程。
   * 		如果这个线程把this指针delele了，或者改了什么东西，那么TcpServer所在线程就会出错。
	 * 为什么不在TcpServer所在线程执行以满足线程安全性(TcpConnection就是由TcpServer所在线程创建的)
   *    1.只有TcpConnection自己知道自己什么时候需要关闭，TcpServer哪里会知道
   *    2.一旦需要关闭，就必定需要将自己从TcpServer的connections_中移除，还是暴露了TcpServer
   *    3.这里仅仅让一条语句变为线程不安全的，已经尽可能将线程不安全的函数缩短了，然后直接用TcpServer所在线程调用删除操作转为线程安全
   */
  m_loop->RunInLoop(
                    std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

/*
 * 这个函数不是线程安全的，但它是由TcpServer所在事件驱动循环调用的
 */
void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
  m_loop->AssertInLoopThread();
  LOG_INFO << "TcpServer::removeConnectionInLoop [" << m_name << "] - connection " << conn->getName();
  size_t n = m_connections.erase(conn->getName());   (void)n;
  assert(n == 1);
  EventLoop* ioLoop = conn->GetLoop();

	/*
	 * 1. 注意 std::bind这里是值绑定了conn，也就是说conn会复制一份到bind上，conn的引用计数加1。
	 * 		这样可以延长TcpConnection生命期，否则这个函数返回到TcpConnection::handleClos结束时，TcpConnection就会直接销毁。
	 * 2. 为什么不能用RunInLoop(), why? 
	 *		用RunInLoop()可能会立即执行TcpConnection::ConnectDestroyed回调，这样conn还是会直接销毁。
	 */
  ioLoop->QueueInLoop(
                      std::bind(&TcpConnection::ConnectDestroyed, conn));
}

