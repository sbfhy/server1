#include "src/gateserver/gateserver.h"

#include "src/base/mgr/mgr_message.h"
#include "src/base/mgr/mgr_dynamicfactory.h"

void GateServer::tick(QWORD usec)
{
}

void GateServer::registerMgrs()
{
    addMgr(MgrDynamicFactory::PInstance());
    addMgr(MgrMessage::PInstance());
}