#pragma once

#include "service/c2g_user.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {
    
class C2G_UserService : public CreateDynamicEmpty<C2G_UserService>
                      , public CMD::C2G_UserService
{
public:
    C2G_UserService() {}

    virtual void C2G_UserSignIn(const ::CMD::C2G_UserSignInArgPtr& request,
                                const ::CMD::C2G_UserSignInResPtr& response) override;

};

}   // namespace RPC
