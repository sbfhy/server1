#include "mgr_user.h"

#include "muduo/base/common/logging.h"
#include "user.h"

bool MgrUser::UserLogin(QWORD accid, RpcChannelPtr rpcChannelPtr)
{
    auto itFind = m_mapUser.find(accid);
    if (itFind == m_mapUser.end()) 
    {
        {LOG_DEBUG << "重复登录，accid:" << accid;}
        return false;
    }
    m_mapUser[accid] = std::make_shared<User>(accid, rpcChannelPtr);
    return true;
}

bool MgrUser::CheckUserRpcChannel(QWORD accid, RpcChannelPtr rpcChannelPtr)
{
    auto itFind = m_mapUser.find(accid);
    if (itFind == m_mapUser.end() || !itFind->second) 
        return false;
    return itFind->second->GetRpcChannelPtr() == rpcChannelPtr;
}
