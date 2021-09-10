#pragma once

#include "service/c2a_user.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {

class C2A_UserService : public CreateDynamicEmpty<C2A_UserService>
                      , public CMD::C2A_UserService
{
public:
    C2A_UserService();

    virtual void C2A_UserLogin(const ::CMD::C2A_UserLoginArgPtr& request,
                               const ::CMD::C2A_UserLoginResPtr& response) override;

};

}   // namespace RPC
