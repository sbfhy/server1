#include "src/gameserver/gameserver.h"

#include "src/base/mgr/mgr_message.h"
#include "src/gameserver/scene/mgr_scene.h"

void GameServer::tick(QWORD usec)
{
}

void GameServer::registerMgrs()
{
    addMgr(MgrMessage::PInstance());
    addMgr(MgrScene::PInstance());
}