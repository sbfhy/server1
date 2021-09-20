#include "user.h"

#include "message/common/rpc.pb.h"

void User::Send(const CMD::RpcMessage& rpcMsg)
{
    if (m_rpcChannelPtr)
    {
        m_rpcChannelPtr->Send(rpcMsg);
    }
}
