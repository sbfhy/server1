#pragma once

#include "muduo/base/common/mutex.h"
#include "muduo/net/common/tcp_connection.h"
#include <string>

using std::string;

namespace muduo
{
namespace net
{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : noncopyable
{
public:
  TcpClient(EventLoop* loop, const InetAddress& serverAddr, const string& nameArg);
  virtual ~TcpClient();  // force out-line dtor, for std::unique_ptr members.

  void connect();
  void disconnect();
  void stop();

  TcpConnectionPtr getConnection() const { MutexLockGuard lock(m_mutex); return m_connection; }

  EventLoop* getLoop() const { return m_loop; }
  bool getRetry() const { return m_retry; }
  void enableRetry() { m_retry = true; }

  const string& getName() const { return m_name; }
  /// Set connection callback. Not thread safe.
  void setConnectionCallback(ConnectionCallback cb) { m_connectionCallback = std::move(cb); }
  /// Set message callback. Not thread safe.
  void setMessageCallback(MessageCallback cb) { m_messageCallback = std::move(cb); }
  /// Set write complete callback. Not thread safe.
  void setWriteCompleteCallback(WriteCompleteCallback cb) { m_writeCompleteCallback = std::move(cb); }

private:
  /// Not thread safe, but in loop
  void newConnection(int sockfd);
  /// Not thread safe, but in loop
  void removeConnection(const TcpConnectionPtr& conn);

private:
  EventLoop* m_loop;
  ConnectorPtr m_connector;                             // avoid revealing Connector
  const string m_name;
  ConnectionCallback m_connectionCallback;
  MessageCallback m_messageCallback;
  WriteCompleteCallback m_writeCompleteCallback;
  bool m_retry;   // atomic
  bool m_connect; // atomic
  // always in loop thread
  int m_nextConnId;
  mutable MutexLock m_mutex;
  TcpConnectionPtr m_connection GUARDED_BY(m_mutex);
};

}  // namespace net
}  // namespace muduo

