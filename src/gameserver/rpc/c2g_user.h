#pragma once

#include "service/c2g_user.pb.h"

#define PROTOBUF_NAMESPACE_ID google::protobuf

namespace RPC {

class C2G_UserLoginServiceImpl : public CMD::C2G_UserLoginService
{
    virtual void C2G_UserLogin(::PROTOBUF_NAMESPACE_ID::RpcController *controller,
                               const ::CMD::C2G_UserLoginArg *request,
                               ::CMD::C2G_UserLoginRes *response,
                               ::google::protobuf::Closure *done) override;
};

}   // namespace RPC
