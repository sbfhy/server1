#pragma once

#include "muduo/net/common/tcp_server.h"

class EchoServer
{
public:
  EchoServer(muduo::net::EventLoop* loop
             , const muduo::net::InetAddress& listenAddr);

  void start();   // calls m_server.start();
  
private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);

  void onMessage(const muduo::net::TcpConnectionPtr& conn
                 , muduo::net::Buffer* buf
                 , muduo::TimeStamp time);

private:
  muduo::net::TcpServer m_server;
};

