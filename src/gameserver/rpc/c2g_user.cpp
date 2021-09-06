#include "src/gameserver/rpc/c2g_user.h"

#include "muduo/base/common/logging.h"

using namespace RPC;


C2G_UserService::C2G_UserService()
{

}

void C2G_UserService::C2G_UserLogin(const ::CMD::C2G_UserLoginArgPtr &request,
                                    const ::CMD::C2G_UserLoginResPtr& response)
{
    if (request)
    {
        {LDBG("M_NET") << request->ShortDebugString();}
    }
    if (response)
    {
        response->set_allow_login(true);
    }
}
