#include "muduo/net/common/tcp_client.h"
#include "muduo/base/common/logging.h"
#include "muduo/base/define/define_new.h"
#include "muduo/net/common/connector.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/sockets_ops.h"
#include <stdio.h>  // snprintf

using namespace muduo;
using namespace muduo::net;

namespace muduo
{
namespace net
{
namespace detail
{

void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn)
{
  loop->QueueInLoop(
                    std::bind(&TcpConnection::ConnectDestroyed, conn));
}

void removeConnector(const ConnectorPtr& connector)
{
  //connector->
}

}  // namespace detail
}  // namespace net
}  // namespace muduo

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr, const string& nameArg)
  : m_loop(CHECK_NOTNULL(loop))
  , m_connector(NEW Connector(loop, serverAddr))
  , m_name(nameArg)
  , m_connectionCallback(defaultConnectionCallback)
  , m_messageCallback(defaultMessageCallback)
  , m_retry(false)
  , m_connect(true)
  , m_nextConnId(1)
{
  m_connector->setNewConnectionCallback(
                                        std::bind(&TcpClient::newConnection, this, _1));
  // FIXME setConnectFailedCallback
  LOG_INFO << "TcpClient::TcpClient[" << m_name << "] - connector " << get_pointer(m_connector);
}

TcpClient::~TcpClient()
{
  LOG_INFO << "TcpClient::~TcpClient[" << m_name << "] - connector " << get_pointer(m_connector);
  TcpConnectionPtr conn;
  bool unique = false;
  {
    MutexLockGuard lock(m_mutex);
    unique = m_connection.unique();
    conn = m_connection;
  }
  if (conn)
  {
    assert(m_loop == conn->getLoop());
    // FIXME: not 100% safe, if we are in different thread
    CloseCallback cb = std::bind(&detail::removeConnection, m_loop, _1);
    m_loop->RunInLoop(
                      std::bind(&TcpConnection::setCloseCallback, conn, cb));
    if (unique)
    {
      conn->ForceClose();
    }
  }
  else
  {
    m_connector->stop();
    // FIXME: HACK
    m_loop->RunAfter(1, 
                     std::bind(&detail::removeConnector, m_connector));
  }
}

void TcpClient::connect()
{
  // FIXME: check state
  LOG_INFO << "TcpClient::connect[" << m_name << "] - connecting to " << m_connector->getServerAddress().toIpPort();
  m_connect = true;
  m_connector->start();
}

void TcpClient::disconnect()
{
  m_connect = false;
  {
    MutexLockGuard lock(m_mutex);
    if (m_connection)
    {
      m_connection->shutdown();
    }
  }
}

void TcpClient::stop()
{
  m_connect = false;
  m_connector->stop();
}

void TcpClient::newConnection(int sockfd)
{
  m_loop->AssertInLoopThread();
  InetAddress peerAddr(sockets::getPeerAddr(sockfd));
  char buf[32];
  snprintf(buf, sizeof buf, ":%s#%d", peerAddr.toIpPort().c_str(), m_nextConnId);
  ++m_nextConnId;
  string connName = m_name + buf;

  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(NEW TcpConnection(m_loop, connName, sockfd, localAddr, peerAddr));
  conn->setConnectionCallback(m_connectionCallback);
  conn->setMessageCallback(m_messageCallback);
  conn->setWriteCompleteCallback(m_writeCompleteCallback);
  conn->setCloseCallback(std::bind(&TcpClient::removeConnection, this, _1)); // FIXME: unsafe
  {
    MutexLockGuard lock(m_mutex);
    m_connection = conn;
  }
  conn->ConnectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
  m_loop->AssertInLoopThread();
  assert(m_loop == conn->getLoop());

  {
    MutexLockGuard lock(m_mutex);
    assert(m_connection == conn);
    m_connection.reset();
  }

  m_loop->QueueInLoop(std::bind(&TcpConnection::ConnectDestroyed, conn));
  if (m_retry && m_connect)
  {
    LOG_INFO << "TcpClient::connect[" << m_name << "] - Reconnecting to " << m_connector->getServerAddress().toIpPort();
    m_connector->restart();
  }
}

