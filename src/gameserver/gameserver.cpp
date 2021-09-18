#include "src/gameserver/gameserver.h"

#include "src/gameserver/scene/mgr_scene.h"

GameServer::GameServer(const ServerArgs& args) 
    : Server(args) 
{
}

void GameServer::tick(QWORD usec)
{
}

void GameServer::registerMgrs()
{
    Server::registerMgrs();
    addMgr(MgrScene::PInstance());
}