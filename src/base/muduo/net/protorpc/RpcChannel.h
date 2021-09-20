#pragma once

#include <google/protobuf/stubs/common.h> // implicit_cast, down_cast
#if GOOGLE_PROTOBUF_VERSION >= 3000000
#include <google/protobuf/stubs/casts.h> // implicit_cast, down_cast
#endif

#include "muduo/base/common/atomic.h"
#include "muduo/base/common/mutex.h"
#include "muduo/net/protorpc/RpcCodec.h"
#include "muduo/net/common/timerid.h"
#include "define/define_service.h"
#include "down_pointer_cast.h"
#include "service.h"

#include <map>

// Service and RpcChannel classes are incorporated from
// google/protobuf/service.h

// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

namespace google {
namespace protobuf {

// Defined in other files.
class Descriptor;        // descriptor.h
class ServiceDescriptor; // descriptor.h
class MethodDescriptor;  // descriptor.h
class Message;           // message.h

// When you upcast (that is, cast a pointer from type Foo to type
// SuperclassOfFoo), it's fine to use implicit_cast<>, since upcasts
// always succeed.  When you downcast (that is, cast a pointer from
// type Foo to type SubclassOfFoo), static_cast<> isn't safe, because
// how do you know the pointer is really of type SubclassOfFoo?  It
// could be a bare Foo, or of type DifferentSubclassOfFoo.  Thus,
// when you downcast, you should use this macro.  In debug mode, we
// use dynamic_cast<> to double-check the downcast is legal (we die
// if it's not).  In normal mode, we do the efficient static_cast<>
// instead.  Thus, it's important to test in debug mode to make sure
// the cast is legal!
//    This is the only place in the code we should use dynamic_cast<>.
// In particular, you SHOULDN'T be using dynamic_cast<> in order to
// do RTTI (eg code like this:
//    if (dynamic_cast<Subclass1>(foo)) HandleASubclass1Object(foo);
//    if (dynamic_cast<Subclass2>(foo)) HandleASubclass2Object(foo);
// You should design the code some other way not to need this.

// template<typename To, typename From>     // use like this: down_pointer_cast<T>(foo);
// inline ::std::shared_ptr<To> down_pointer_cast(const ::std::shared_ptr<From>& f) {
//   // so we only accept smart pointers
//   // Ensures that To is a sub-type of From *.  This test is here only
//   // for compile-time type checking, and has no overhead in an
//   // optimized build at run-time, as it will be optimized away
//   // completely.
//   if (false) {
//     implicit_cast<const From*, To*>(0);
//   }

// #if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
//   assert(f == NULL || dynamic_cast<To*>(muduo::get_pointer(f)) != NULL);  // RTTI: debug mode only!
// #endif
//   return ::std::static_pointer_cast<To>(f);
// }

} // namespace protobuf
} // namespace google

namespace CMD {
    class RpcMessage;
}

namespace muduo
{
namespace net
{

class RpcController;
class Service;
class EventLoop;

// Abstract interface for an RPC channel.  An RpcChannel represents a
// communication line to a Service which can be used to call that Service's
// methods.  The Service may be running on another machine.  Normally, you
// should not call an RpcChannel directly, but instead construct a stub Service
// wrapping it.  Example:
// FIXME: update here
//   RpcChannel* channel = new MyRpcChannel("remotehost.example.com:1234");
//   MyService* service = new MyService::Stub(channel);
//   service->MyMethod(request, &response, callback);
class RpcChannel : public RpcChannelBase
                 , public std::enable_shared_from_this<RpcChannel>
{
public:
    RpcChannel();

    explicit RpcChannel(const TcpConnectionPtr &conn);

    virtual ~RpcChannel();

    void setConnection(const TcpConnectionPtr &conn) { m_conn = conn; }

    void Send(const ::google::protobuf::MessagePtr& request, QWORD accid = 0);
    void Send(const CMD::RpcMessage& rpcMsg);

    // typedef ::std::function<void(const ::google::protobuf::MessagePtr &)> ClientDoneCallback;

    void onDisconnect();

    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buf,
                   TimeStamp receiveTime);

    void SetAccid(QWORD accid) { m_accid = accid; }
    QWORD GetAccid() const { return m_accid; }

protected:
    void onRpcMessage(const TcpConnectionPtr &conn,
                      const CMD::RpcMessagePtr &messagePtr,
                      TimeStamp receiveTime);
    void serviceHandleRequestMsg(const CMD::RpcMessage &message);    // Service处理request消息
    void stubHandleResponseMsg(const CMD::RpcMessage &message);      // Stub处理response消息

private:
    void onRpcMessageInMainLoop(const TcpConnectionPtr &conn,
                                const CMD::RpcMessagePtr &messagePtr,
                                TimeStamp receiveTime);

    void doneCallbackInIoLoop(::google::protobuf::MessagePtr response,
                              int64_t id,
                              uint64_t accid,
                              ENUM::EServerType from);
    void doneCallback(::google::protobuf::MessagePtr response,
                      int64_t id,
                      uint64_t accid,
                      ENUM::EServerType from);

    void requestTimeOut(int64_t id);

protected:
    struct OutstandingCall
    {
        ::google::protobuf::MessagePtr request = nullptr;
        ENUM::EServiceType serviceType = ENUM::SERVICETYPE_MIN;
        int methodIndex = -1;
        TimerId timerId;
    };

    RpcCodec m_codec;
    TcpConnectionPtr m_conn;
    AtomicInt64 m_id;                   // 自增Id，回调函数需要

    MutexLock m_mutex;
    std::map<int64_t, OutstandingCall> m_outstandings;

    QWORD m_accid {0};                  // GateServer用
};

}
}

typedef std::shared_ptr<muduo::net::RpcChannel> RpcChannelPtr; // FIXME: unique_ptr
