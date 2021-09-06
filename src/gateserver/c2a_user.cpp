#include "c2a_user.h"

#include "mgr_user.h"

using namespace RPC;

C2A_UserService::C2A_UserService()
{

}

void C2A_UserService::C2A_UserLogin(const ::CMD::C2A_UserLoginArgPtr& request,
                                    const ::CMD::C2A_UserLoginResPtr& response) 
{
    if (!request || !response) return;
    response->set_allow_login(
            MgrUser::Instance().UserLogin(request->accid()) );
}
