#include "echo_server.h"
#include "muduo/base/common/logging.h"

using namespace std::placeholders;


EchoServer::EchoServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
  : m_server(loop, listenAddr, "EchoServer")
{
  m_server.setConnectionCallback(
      std::bind(&EchoServer::onConnection, this, _1));
  m_server.setMessageCallback(
      std::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start()
{
  m_server.start();
}

void EchoServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
  LOG_INFO << "EchoServer - " << conn->getPeerAddress().toIpPort() << " -> "
           << conn->getLocalAddress().toIpPort() << " is "
           << (conn->IsConnected() ? "UP" : "DOWN");
}

void EchoServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::TimeStamp time)
{
  std::string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->getName() << " echo " << msg.size() << " bytes, "
           << "data received at " << time.toString();
  conn->send(msg);
}

