#include "src/gameserver/gameserver.h"

#include "src/gameserver/scene/mgr_scene.h"
#include "muduo/net/protorpc/RpcChannel.h"

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

void GameServer::SendToGate(const ::google::protobuf::MessagePtr& request, QWORD accid/* = 0*/)
{
    if (m_gateRpcChannelPtr)
    {
        m_gateRpcChannelPtr->Send(request, accid);
    }
}