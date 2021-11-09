#include "mgr_user.h"

#include "muduo/base/common/logging.h"
#include "user.h"
#include "src/base/mgr/mgr_event_global.h"
#include "define/define_event.h"
#include "src/gateserver/gateserver.h"

#include "message/common/rpc.pb.h"
#include "service/a2g_user.pb.h"

bool MgrUser::Init()
{
    ADD_EVENT_GLOBAL_3(EG_EVENT_TYPE_SIGN_OUT, 
                       this, 
                       EVENT_CALLBACK(MgrUser::UserSignOut, this));
    return true;
}

bool MgrUser::UserSignIn(QWORD accid, RpcChannelPtr rpcChannelPtr)
{
    auto itFind = m_mapUser.find(accid);
    if (itFind != m_mapUser.end()) 
    {
        {LOG_DEBUG << "重复登录，accid:" << accid;}
        return false;
    }
    m_mapUser[accid] = std::make_shared<User>(accid, rpcChannelPtr);
    LOG_DEBUG << "用户上线, accid:" << accid;
    return true;
}

bool MgrUser::CheckUserRpcChannel(QWORD accid, RpcChannelPtr rpcChannelPtr)
{
    auto itFind = m_mapUser.find(accid);
    if (itFind == m_mapUser.end() || !itFind->second) 
        return false;
    return itFind->second->GetRpcChannelPtr() == rpcChannelPtr;
}

void MgrUser::UserSignOut(xEntry* sender, const IVarList& varList)
{
    QWORD accid = varList.qwordVal(0);
    m_mapUser.erase(accid);
    LOG_DEBUG << "用户下线, accid:" << accid;
    
    auto A2G_UserSignOutArgPtr = std::make_shared<CMD::A2G_UserSignOutArg>();
    A2G_UserSignOutArgPtr->set_accid(accid);
    thisServer->SendToGame(A2G_UserSignOutArgPtr);
}

void MgrUser::Send(const CMD::RpcMessage& rpcMsg)
{
    auto itFind = m_mapUser.find(rpcMsg.accid());
    if (itFind == m_mapUser.end() || !itFind->second)
        return;
    itFind->second->Send(rpcMsg);
}
