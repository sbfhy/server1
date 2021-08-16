#pragma once

#include "muduo/base/common/atomic.h"
#include "muduo/net/common/tcp_connection.h"
#include <map>

namespace muduo
{
namespace net
{

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

///
/// TCP server, supports single-threaded and thread-pool models.
///
/// This is an interface class, so don't expose too much details.
class TcpServer : noncopyable
{
public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;
  enum Option                           // 是否打开Acceptor监听套接字的SO_REUSEPORT选项
  {
    kNoReusePort,
    kReusePort,
  };

  TcpServer(EventLoop* loop, const InetAddress& listenAddr, const string& nameArg, Option option = kNoReusePort);
  virtual ~TcpServer();  // force out-line dtor, for std::unique_ptr members.

  const string& getIpPort() const { return m_ipPort; }
  const string& getName() const { return m_name; }
  EventLoop* getLoop() const { return m_loop; }

  /// Set the number of threads for handling input.
  ///
  /// Always accepts new connection in loop's thread.
  /// Must be called before @c start
  /// @param numThreads
  /// - 0 means all I/O in loop's thread, no thread will created.
  ///   this is the default value.
  /// - 1 means all I/O in another thread.
  /// - N means a thread pool with N threads, new connections
  ///   are assigned on a round-robin basis.
  void setThreadNum(int numThreads);
  void setThreadInitCallback(const ThreadInitCallback& cb) { m_threadInitCallback = cb; }
  /// valid after calling start()
  std::shared_ptr<EventLoopThreadPool> getThreadPool() { return m_threadPool; }

  /// Starts the server if it's not listenning.
  ///
  /// It's harmless to call it multiple times.
  /// Thread safe.
  void start();   // 调用m_threadPool->start(1), m_acceptor->listen()

  /// Set connection callback.
  /// Not thread safe.
  void setConnectionCallback(const ConnectionCallback& cb) { m_connectionCallback = cb; }

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(const MessageCallback& cb) { m_messageCallback = cb; }

  /// Set write complete callback.
  /// Not thread safe.
  void setWriteCompleteCallback(const WriteCompleteCallback& cb) { m_writeCompleteCallback = cb; }

private:
  /*
   * Not thread safe, but in loop
   * m_acceptor有新连接过来时回调到这个函数。
   * 会新建一个TcpConnection，并设置它的回调函数，然后调用TcpConnection::ConnectEstablished。
   */
  void newConnection(int sockfd, const InetAddress& peerAddr);
  
  /* Thread safe.
   * 关闭连接时回调到这里，会删除这个TcpConnection 
   *    当poll返回后，发现被激活的原因是EPOLLHUP，此时需要关闭tcp连接。
   *    调用Channel的CloseCallback，进而调用TcpConnection的handleClose，进而调用removeConnection
   */
  void removeConnection(const TcpConnectionPtr& conn);

  /// Not thread safe, but in loop
  void removeConnectionInLoop(const TcpConnectionPtr& conn);

private:
  typedef std::map<string, TcpConnectionPtr> ConnectionMap;

  EventLoop* m_loop;  // the acceptor loop                            // 与m_acceptor是同一个loop，即监听线程
  const string m_ipPort;                                              // ip和端口
  const string m_name;
  std::unique_ptr<Acceptor> m_acceptor;                               // avoid revealing Acceptor   
  std::shared_ptr<EventLoopThreadPool> m_threadPool;                  // 事件驱动线程池

  // 回调函数，由用户设置
  ConnectionCallback m_connectionCallback;                            // 
  MessageCallback m_messageCallback;                                  // 
  WriteCompleteCallback m_writeCompleteCallback;                      // 

  ThreadInitCallback m_threadInitCallback;                            // 线程初始化的回调函数

  AtomicInt32 m_started;                                              // 
  // always in loop thread
  int m_nextConnId;                                                   // 用于设置下一个创建的TcpConnection的序号
  ConnectionMap m_connections;    // 记录创建出来的TcpConnection，< (服务器名-<客户端ip, 客户端端口>#序号), >
};

}  // namespace net
}  // namespace muduo

