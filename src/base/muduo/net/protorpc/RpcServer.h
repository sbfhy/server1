// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#pragma once

#include "muduo/base/define/define_service.h"
#include "muduo/net/common/tcp_server.h"

namespace muduo
{
namespace net
{

class RpcServer
{
 public:
  RpcServer(EventLoop* loop,
            const InetAddress& listenAddr);
  RpcServer(std::shared_ptr<EventLoop> mainLoop, 
            EventLoop* loop, 
            const InetAddress& listenAddr);

  void setThreadNum(int numThreads)
  {
    server_.setThreadNum(numThreads);
  }

  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);

  std::shared_ptr<EventLoop> m_mainLoop{nullptr};
  // EventLoop* loop_;
  TcpServer server_;
};

}
}

