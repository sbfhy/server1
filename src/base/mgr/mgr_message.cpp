#include "src/base/mgr/mgr_message.h"

#include "src/gameserver/rpc/c2g_user.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

void MgrMessage::Wake()
{
    registerService();
}

void MgrMessage::registerService(ServicePtr service)
{
    const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
    m_mapServices[desc->full_name()] = service;
}

void MgrMessage::registerService()
{
    registerService(std::make_shared<RPC::C2G_UserLoginServiceImpl>());
}
