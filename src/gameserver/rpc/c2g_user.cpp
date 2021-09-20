#include "src/gameserver/rpc/c2g_user.h"

#include "muduo/base/common/logging.h"

using namespace RPC;

void C2G_UserService::C2G_UserSignIn(const ::CMD::C2G_UserSignInArgPtr &request,
                                     const ::CMD::C2G_UserSignInResPtr& response)
{
    if (request)
    {
        {LDBG("M_NET") << request->ShortDebugString();}
    }
    if (response)
    {
        response->set_allow_sign_in(true);
    }
}
