#include "muduo/net/common/acceptor.h"
#include "muduo/base/common/logging.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/inet_address.h"
#include "muduo/net/common/sockets_ops.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport)
  : m_loop(loop)
  , m_acceptSocket(sockets::createNonblockingOrDie(listenAddr.family()))
  , m_acceptChannel(loop, m_acceptSocket.fd())
  , m_listenning(false)
  , m_idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
  assert(m_idleFd >= 0);
  m_acceptSocket.setReuseAddr(true);	// 打开套接字选项SO_REUSEADDR，可以使用还处于TIME_WAIT状态的端口
  m_acceptSocket.setReusePort(reuseport);	// 允许多个套接字 bind()/listen() 同一个TCP/UDP端口；每个线程一个套接字，避免锁竞争
  m_acceptSocket.bindAddress(listenAddr);
  m_acceptChannel.setReadCallback(
																	std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
  m_acceptChannel.DisableAll();
  m_acceptChannel.Remove();
  ::close(m_idleFd);
}

void Acceptor::listen()
{
  m_loop->AssertInLoopThread();
  m_listenning = true;
  m_acceptSocket.listen();								// 转为监听套接字
  m_acceptChannel.EnableReading();				// 该套接字变为可读，这里会把它注册到Poller中
}

void Acceptor::handleRead()
{
  m_loop->AssertInLoopThread();
  InetAddress peerAddr;
  //FIXME loop until no more
  int connfd = m_acceptSocket.accept(&peerAddr);
  if (connfd >= 0)
  {
    if (m_newConnectionCallback)
    {
      m_newConnectionCallback(connfd, peerAddr);	// 这里的回调函数是TcpServer中的newConnection，用于创建一个TcpConnection连接
    }
    else
    {
      sockets::close(connfd);
    }
  }
  else
  {
    LOG_SYSERR << "in Acceptor::handleRead";
    // Read the section named "The special problem of accept()ing when you can't" in libev's doc.
    // By Marc Lehmann, author of libev.
    if (errno == EMFILE)
    {
      ::close(m_idleFd);
      m_idleFd = ::accept(m_acceptSocket.fd(), nullptr, nullptr);
      ::close(m_idleFd);
      m_idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
    }
  }
}

