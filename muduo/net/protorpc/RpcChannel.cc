// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/protorpc/RpcChannel.h"
#include "muduo/net/protorpc/service.h"

#include "muduo/base/common/logging.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/buffer.h"
#include "muduo/net/common/tcp_connection.h"
#include "rpc.pb.h"

#include <google/protobuf/descriptor.h>

using namespace muduo;
using namespace muduo::net;

static int test_down_pointer_cast()
{
  ::std::shared_ptr< ::google::protobuf::Message> msg(new RpcMessage);
  ::std::shared_ptr<RpcMessage> rpc(::google::protobuf::down_pointer_cast<RpcMessage>(msg));
  assert(msg && rpc);
  if (!rpc)
  {
    abort();
  }
  return 0;
}

static int dummy __attribute__ ((unused)) = test_down_pointer_cast();

RpcChannel::RpcChannel()
  : codec_(std::bind(&RpcChannel::onRpcMessageInMainLoop, this, _1, _2, _3)),
    services_(NULL)
{
  LOG_INFO << "RpcChannel::ctor - " << this;
}

RpcChannel::RpcChannel(const TcpConnectionPtr& conn)
  : codec_(std::bind(&RpcChannel::onRpcMessageInMainLoop, this, _1, _2, _3)),
    conn_(conn),
    services_(NULL)
{
  LOG_INFO << "RpcChannel::ctor - " << this;
}

RpcChannel::~RpcChannel()
{
  LOG_INFO << "RpcChannel::dtor - " << this;
}

// Call the given method of the remote service.  The signature of this
// procedure looks the same as Service::CallMethod(), but the requirements
// are less strict in one important way:  the request and response objects
// need not be of any specific class as long as their descriptors are
// method->input_type() and method->output_type().
void RpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                            google::protobuf::RpcController* controller,
                            const ::google::protobuf::Message* request,
                            ::google::protobuf::Message* response,
                            ::google::protobuf::Closure* done)
{
  // FIXME: can we move serialization to IO thread?
  RpcMessage message;
  message.set_type(MSGTYPE_REQUEST);
  int64_t id = id_.incrementAndGet();
  message.set_id(id);
  message.set_service(method->service()->full_name());
  message.set_method(method->index());
  message.set_request(request->SerializeAsString()); // FIXME: error check

  OutstandingCall out = { response, done };
  {
  MutexLockGuard lock(mutex_);
  outstandings_[id] = out;
  }
  codec_.send(conn_, message);
}

void RpcChannel::onDisconnect()
{
  //FIXME:
}

void RpcChannel::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           TimeStamp receiveTime)
{
  LOG_TRACE << "RpcChannel::onMessage " << buf->readableBytes();
  codec_.onMessage(conn, buf, receiveTime);
}

void RpcChannel::onRpcMessage(const TcpConnectionPtr& conn,
                              const RpcMessagePtr& messagePtr,
                              TimeStamp receiveTime)
{
  assert(conn == conn_);
  //printf("%s\n", message.DebugString().c_str());
  RpcMessage& message = *messagePtr;
  LOG_TRACE << "RpcChannel::onRpcMessage " << message.DebugString();
  if (message.type() == MSGTYPE_RESPONSE)
  {
    int64_t id = message.id();
    assert(message.response() != "");

    OutstandingCall out = { NULL, NULL };
    bool found = false;

    {
      MutexLockGuard lock(mutex_);
      std::map<int64_t, OutstandingCall>::iterator it = outstandings_.find(id);
      if (it != outstandings_.end())
      {
        out = it->second;
        outstandings_.erase(it);
        found = true;
      }
      else
      {
#ifndef NDEBUG
        LOG_WARN << "Size " << outstandings_.size();
        for (it = outstandings_.begin(); it != outstandings_.end(); ++it)
        {
          LOG_WARN << "id " << it->first;
        }
#endif
      }
    }

    if (!found)
    {
      LOG_ERROR << "Unknown MSGTYPE_RESPONSE";
    }

    if (out.response)
    {
      // FIXME: can we move deserialization to other thread?
      ::google::protobuf::MessagePtr response(out.response); // 回调用完就把out.response释放掉
      out.response->ParseFromString(message.response());
      if (out.done)
      {
        // out.done(response);
        out.done->Run();
      }
    }
    else
    {
      LOG_ERROR << "No Response prototype";
    }
  }
  else if (message.type() == MSGTYPE_REQUEST)
  {
    callServiceMethod(message);
  }
  else if (message.type() == MSGTYPE_ERROR)
  {
    // FIXME:
  }
}
  
void RpcChannel::onRpcMessageInMainLoop(const TcpConnectionPtr& conn,
                                        const RpcMessagePtr& messagePtr,
                                        TimeStamp receiveTime)
{
  std::shared_ptr<EventLoop> mainLoopPtr = conn->getMainLoop();
  if (mainLoopPtr)
  {
    mainLoopPtr->RunInLoop( 
        std::bind(&RpcChannel::onRpcMessage, this, conn, messagePtr, receiveTime) );
  }
  else
  {
    onRpcMessage(conn, messagePtr, receiveTime);
  }
}

void RpcChannel::callServiceMethod(const RpcMessage& message)   // 处理request消息
{
  ErrorCode errorCode = ERR_WRONG_PROTO;
  if (services_)
  {
    ServiceMap::const_iterator it = services_->find(message.service());
    if (it != services_->end())
    {
      google::protobuf::Service* service = it->second;
      assert(service != NULL);
      const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
      const google::protobuf::MethodDescriptor* method
          = desc->method(message.method());
      if (method)
      {
        // FIXME: can we move deserialization to other thread?
        ::google::protobuf::MessagePtr request(service->GetRequestPrototype(method).New());
        if (request->ParseFromString(message.request()))
        {
          int64_t id = message.id();
          ::google::protobuf::Message *response = service->GetResponsePrototype(method).New();
          // response is deleted in doneCallbac
          service->CallMethod(method, nullptr, get_pointer(request), response,
                              NewCallback(this, &RpcChannel::doneCallbackInIoLoop, response, id));
          errorCode = ERR_NO_ERROR;
        }
      }
      else
      {
        // FIXME:
        errorCode = ERR_NO_METHOD;
      }
    }
    else
    {
      // FIXME:
      errorCode = ERR_NO_SERVICE;
    }
  }
  else
  {
    // FIXME:
    errorCode = ERR_NO_SERVICE;
  }

  if (errorCode != ERR_NO_ERROR)          // 告诉对方处理request时出现错误
  {
    RpcMessage response;
    response.set_type(MSGTYPE_RESPONSE);
    response.set_id(message.id());
    response.set_error(errorCode);
    codec_.send(conn_, response);
  }
}
  
void RpcChannel::doneCallbackInIoLoop(::google::protobuf::Message* response,
                                      int64_t id)
{
  EventLoop* pIoLoop = conn_->getLoop();
  if (pIoLoop)
  {
    pIoLoop->RunInLoop( 
        std::bind(&RpcChannel::doneCallback, this, response, id) );
  }
  else
  {
    doneCallback(response, id);
  }
}

void RpcChannel::doneCallback(::google::protobuf::Message* response,
                              int64_t id)
{
  // FIXME: can we move serialization to IO thread?
  ::google::protobuf::MessagePtr d(response);
  RpcMessage message;
  message.set_type(MSGTYPE_RESPONSE);
  message.set_id(id);
  message.set_response(response->SerializeAsString()); // FIXME: error check
  codec_.send(conn_, message);
  LOG_DEBUG << "size:" << message.ByteSizeLong() << ", " << message.ShortDebugString();
}

