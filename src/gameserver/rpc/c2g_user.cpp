#include "src/gameserver/rpc/c2g_user.h"

#include "muduo/base/common/logging.h"

using namespace RPC;

void C2G_UserLoginServiceImpl::C2G_UserLogin(
        ::PROTOBUF_NAMESPACE_ID::RpcController* controller,
        const ::CMD::C2G_UserLoginArg* request,
        ::CMD::C2G_UserLoginRes* response,
        ::google::protobuf::Closure* done)
{
    if (request)
    {
        LDBG("M_NET") << request->ShortDebugString();
    }
    if (response)
    {
        response->set_allow_login(true);
    }
    if (done)
    {
        done->Run();
    }
}
