#include "a2g_user.h"

#include "muduo/base/common/logging.h"
#include "src/gameserver/user/mgr_user.h"
#include "src/gameserver/gameserver.h"
#include "muduo/net/protorpc/RpcChannel.h"
#include "service/g2c_scene.pb.h"
#include "src/gameserver/user/user.h"

using namespace RPC;

void A2G_UserService::A2G_GateNotifyGame(const ::CMD::A2G_GateNotifyGameArgPtr& request,
                                         const ::CMD::EmptyResponsePtr& response,
                                         void* args)
{
    {LDBG("M_NET") << "Gate通知Game，连接上了";}
    thisServer->SetGateRpcChannelPtr(static_cast<SRpcChannelMethodArgs*>(args)->rpcChannelPtr);
}

void A2G_UserService::A2G_UserSignIn(const ::CMD::A2G_UserSignInArgPtr& request,
                                     const ::CMD::EmptyResponsePtr& response,
                                     void* args)
{
    if (!request) return ;
    {LDBG("M_NET") << request->ShortDebugString();}
    
    QWORD accid = request->accid();
    MgrUser::Instance().UserSignIn(accid);
}

void A2G_UserService::A2G_UserSignOut(const ::CMD::A2G_UserSignOutArgPtr& request,
                                      const ::CMD::EmptyResponsePtr& response,
                                      void* args)
{
    if (!request) return ;
    QWORD accid = request->accid();
    MgrUser::Instance().UserSignOut(accid);
}
