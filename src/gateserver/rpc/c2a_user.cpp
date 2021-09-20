#include "c2a_user.h"

#include "src/gateserver/mgr_user.h"
#include "muduo/net/protorpc/RpcChannel.h"
#include "muduo/base/common/logging.h"
#include "src/gateserver/gateserver.h"
#include "service/a2g_user.pb.h"

using namespace RPC;

void C2A_UserService::C2A_UserSignIn(const ::CMD::C2A_UserSignInArgPtr& request,
                                     const ::CMD::C2A_UserSignInResPtr& response) 
{
    if (!request || !response) return;
    LOG_DEBUG << request->ShortDebugString();
    RpcChannelPtr rpcChannelPtr = std::static_pointer_cast<muduo::net::RpcChannel>(GetRpcChannel());
    if (!rpcChannelPtr) return ;

    QWORD accid = request->accid();
    bool bAllowSignIn = MgrUser::Instance().UserSignIn( accid, rpcChannelPtr );
    response->set_allow_sign_in(bAllowSignIn);
    if (bAllowSignIn)
    {
        rpcChannelPtr->SetAccid(accid);

        CMD::A2G_UserSignInArgPtr A2G_UserSignInArgPtr = std::make_shared<CMD::A2G_UserSignInArg>();
        A2G_UserSignInArgPtr->set_accid(accid);
        if (thisServer)
        {
            thisServer->SendToGame(A2G_UserSignInArgPtr);
        }
    }
}
