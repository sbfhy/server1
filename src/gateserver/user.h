#pragma once 

#include "muduo/net/protorpc/RpcChannel.h"

namespace CMD {
    class RpcMessage;
}   // namespace CMD

class User
{
public:
    User(QWORD accid) : m_accid(accid) {}
    User(QWORD accid, RpcChannelPtr rpcChannelPtr) : m_accid(accid), m_rpcChannelPtr(rpcChannelPtr) {}
    virtual ~User() = default;

    RpcChannelPtr GetRpcChannelPtr() const { return m_rpcChannelPtr; }
    
    void Send(const CMD::RpcMessage& rpcMsg);

private:
    QWORD m_accid {0};
    RpcChannelPtr m_rpcChannelPtr;
};
