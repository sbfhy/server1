#include "src/gameserver/user/user.h"

#include "src/gameserver/gameserver.h"

void User::Send(const ::google::protobuf::MessagePtr& request)
{
    thisServer->SendToGate(request, m_accid);
}
