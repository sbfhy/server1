#pragma once

#include "service/a2g_user.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {
    
class A2G_UserService : public CreateDynamicEmpty<A2G_UserService>
                      , public CMD::A2G_UserService
{
public:
    A2G_UserService() {}

    virtual void A2G_GateNotifyGame(const ::CMD::A2G_GateNotifyGameArgPtr& request,
                                    const ::CMD::EmptyResponsePtr& response) override;

    virtual void A2G_UserSignIn(const ::CMD::A2G_UserSignInArgPtr& request,
                                const ::CMD::EmptyResponsePtr& response) override;

    virtual void A2G_UserSignOut(const ::CMD::A2G_UserSignOutArgPtr& request,
                                 const ::CMD::EmptyResponsePtr& response) override;
};

}   // namespace RPC
