#include "src/gameserver/gameserver.h"

#include "src/base/mgr/mgr_message.h"
#include "src/base/mgr/mgr_dynamicfactory.h"
#include "src/gameserver/scene/mgr_scene.h"




GameServer::GameServer(const ServerArgs& args) 
    : Server(args) 
    , m_game2gate(this, InetAddress("127.0.0.1", 9981), "game2gate")
{
}

void GameServer::Start()
{
    Server::Start();
    m_game2gate.connect();
}

void GameServer::tick(QWORD usec)
{
}

void GameServer::registerMgrs()
{
    addMgr(MgrDynamicFactory::PInstance());
    addMgr(MgrMessage::PInstance());
    addMgr(MgrScene::PInstance());
}