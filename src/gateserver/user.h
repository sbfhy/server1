#pragma once 

#include "muduo/net/protorpc/RpcChannel.h"

class User
{
public:
    User(QWORD accid) : m_accid(accid) {}
    User(QWORD accid, RpcChannelPtr rpcChannelPtr) : m_accid(accid), m_rpcChannelPtr(rpcChannelPtr) {}
    virtual ~User() = default;

    RpcChannelPtr GetRpcChannelPtr() const { return m_rpcChannelPtr; }

private:
    QWORD m_accid {0};
    RpcChannelPtr m_rpcChannelPtr;
};
