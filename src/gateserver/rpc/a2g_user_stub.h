#pragma once

#include "service/a2g_user.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {

class A2G_UserService_Stub : public CreateDynamicEmpty<A2G_UserService_Stub>
                           , public CMD::A2G_UserService_Stub
{
public:
    A2G_UserService_Stub() {}

};

}   // namespace RPC
