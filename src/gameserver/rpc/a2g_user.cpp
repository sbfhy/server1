#include "a2g_user.h"

#include "muduo/base/common/logging.h"
#include "src/gameserver/user/mgr_user.h"
#include "src/gameserver/gameserver.h"
#include "muduo/net/protorpc/RpcChannel.h"
#include "service/g2c_scene.pb.h"
#include "src/gameserver/user/user.h"

using namespace RPC;

void A2G_UserService::A2G_GateNotifyGame(const ::CMD::A2G_GateNotifyGameArgPtr& request,
                                         const ::CMD::EmptyResponsePtr& response)
{
    {LDBG("M_NET") << "Gate通知Game，连接上了";}
    thisServer->SetGateRpcChannelPtr(std::dynamic_pointer_cast<muduo::net::RpcChannel>(GetRpcChannel()));
}

void A2G_UserService::A2G_UserSignIn(const ::CMD::A2G_UserSignInArgPtr& request,
                                     const ::CMD::EmptyResponsePtr& response)
{
    if (!request) return ;
    {LDBG("M_NET") << request->ShortDebugString();}
    
    QWORD accid = request->accid();
    MgrUser::Instance().UserSignIn(accid);

    auto userPtr = MgrUser::Instance().GetUser(accid);
    if (userPtr)
    {
        auto G2C_EnterSceneArgPtr = std::make_shared<CMD::G2C_EnterSceneArg>();
        userPtr->Send(G2C_EnterSceneArgPtr);
    }
}

void A2G_UserService::A2G_UserSignOut(const ::CMD::A2G_UserSignOutArgPtr& request,
                                      const ::CMD::EmptyResponsePtr& response)
{
    if (!request) return ;
    QWORD accid = request->accid();
    MgrUser::Instance().UserSignOut(accid);
}
