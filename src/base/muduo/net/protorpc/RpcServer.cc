// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/protorpc/RpcServer.h"

#include "muduo/net/protorpc/RpcChannel.h"
#include "muduo/base/common/logging.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

using namespace muduo;
using namespace muduo::net;

RpcServer::RpcServer(EventLoop* loop,
                     const InetAddress& listenAddr)
  : server_(loop, listenAddr, "RpcServer")
{
  server_.setConnectionCallback(
      std::bind(&RpcServer::onConnection, this, _1));
//   server_.setMessageCallback(
//       std::bind(&RpcServer::onMessage, this, _1, _2, _3));

  // registerService(&metaService_);
}

RpcServer::RpcServer(std::shared_ptr<EventLoop> mainLoop, 
                     EventLoop* loop, 
                     const InetAddress& listenAddr)
  : m_mainLoop(mainLoop)
  , server_(loop, listenAddr, "RpcServer")
{
  server_.setConnectionCallback(
      std::bind(&RpcServer::onConnection, this, _1));
}

void RpcServer::start()
{
  server_.start();
}

void RpcServer::onConnection(const TcpConnectionPtr& conn)
{
  LOG_INFO << "RpcServer - " << conn->getPeerAddress().toIpPort() << " -> "
        << conn->getLocalAddress().toIpPort() << " is "
        << (conn->IsConnected() ? "UP" : "DOWN");
  if (conn->IsConnected())
  {
    RpcChannelPtr channel(new RpcChannel(conn));
    conn->setMessageCallback(
        std::bind(&RpcChannel::onMessage, get_pointer(channel), _1, _2, _3));
    conn->setContext(channel);
    conn->SetMainLoop(m_mainLoop);
  }
  else
  {
    conn->setContext(RpcChannelPtr());
    // FIXME:
  }
}
