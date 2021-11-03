#include "src/gameserver/rpc/c2g_user.h"

#include "muduo/base/common/logging.h"

using namespace RPC;

void C2G_UserService::C2G_UserSignIn(const ::CMD::C2G_UserSignInArgPtr &request,
                                     const ::CMD::C2G_UserSignInResPtr& response,
                                     void* args)
{
    if (request)
    {
        {LDBG("M_NET") << request->ShortDebugString();}
    }
    if (response)
    {
        response->set_allow_sign_in(true);
    }

    // RpcChannelPtr rpcChannelPtr = std::static_pointer_cast<muduo::net::RpcChannel>(GetRpcChannel());
    // if (!rpcChannelPtr) return ;
    // SDelayResponse* sDelayResponse = rpcChannelPtr->NewDelayResponse();
    // sDelayResponse->response = response;
}
