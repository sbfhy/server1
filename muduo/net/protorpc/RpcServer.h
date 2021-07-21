// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_PROTORPC2_RPCSERVER_H
#define MUDUO_PROTORPC2_RPCSERVER_H

#include "muduo/net/common/tcp_server.h"

namespace google {
namespace protobuf {

class Service;

}  // namespace protobuf
}  // namespace google

namespace muduo
{
namespace net
{

class Service;

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

  typedef std::shared_ptr<::google::protobuf::Service> ServicePtr;
  typedef std::map<std::string, ServicePtr> ServiceMap;

  // NOT thread safe, must call before start().
  void setServices(const ServiceMap* services)
  {
    services_ = services;
  }
//   void registerService(ServicePtr);

  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);

  // void onMessage(const TcpConnectionPtr& conn,
  //                Buffer* buf,
  //                TimeStamp time);

  std::shared_ptr<EventLoop> m_mainLoop{nullptr};
  // EventLoop* loop_;
  TcpServer server_;

  const ServiceMap*  services_;
  // RpcServiceImpl metaService_;
};

}
}

#endif  // MUDUO_NET_PROTORPC2_RPCSERVER_H
