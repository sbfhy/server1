#pragma once

#include "muduo/net/protorpc/RpcChannel.h"

using namespace muduo::net;

class TcpConnection;

class RpcChannelGate : public RpcChannel
{
public:
    explicit RpcChannelGate(const TcpConnectionPtr &conn) : RpcChannel(conn) {}

protected:
    void serviceHandleRequestMsg(const RpcMessage &message);    // Service处理request消息
    void stubHandleResponseMsg(const RpcMessage &message);      // Stub处理response消息
};
