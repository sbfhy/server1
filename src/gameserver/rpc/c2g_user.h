#pragma once

#include "service/c2g_user.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {
    
class C2G_UserService : public CreateDynamicEmpty<C2G_UserService>
                           , public CMD::C2G_UserService
{
public:
    C2G_UserService();

    virtual void C2G_UserLogin(const ::CMD::C2G_UserLoginArgPtr& request,
                               const ::CMD::C2G_UserLoginResPtr& response) override;

};

}   // namespace RPC
