#include "c2a_user.h"

#include "mgr_user.h"
#include "muduo/net/protorpc/RpcChannel.h"
#include "muduo/base/common/logging.h"

using namespace RPC;

C2A_UserService::C2A_UserService()
{
}

void C2A_UserService::C2A_UserSignIn(const ::CMD::C2A_UserSignInArgPtr& request,
                                     const ::CMD::C2A_UserSignInResPtr& response) 
{
    if (!request || !response) return;
    LOG_DEBUG << request->ShortDebugString();
    RpcChannelPtr rpcChannelPtr = std::static_pointer_cast<muduo::net::RpcChannel>(GetRpcChannel());
    if (!rpcChannelPtr) return ;

    bool bAllowSignIn = MgrUser::Instance().UserSignIn( request->accid(), rpcChannelPtr );
    response->set_allow_sign_in(bAllowSignIn);
    if (bAllowSignIn)
    {
        rpcChannelPtr->SetAccid(request->accid());
    }
}
