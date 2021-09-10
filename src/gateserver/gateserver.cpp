#include "src/gateserver/gateserver.h"

#include "src/base/mgr/mgr_message.h"
#include "src/base/mgr/mgr_dynamicfactory.h"
#include "src/gateserver/mgr_user.h"
#include "message/common/rpc.pb.h"

GateServer::GateServer(const ServerArgs& args) 
    : Server(args) 
    , m_Gate2Game(this, InetAddress("127.0.0.1", 9982))
{

}

void GateServer::Start()
{
    Server::Start();
    m_Gate2Game.Connect();
}

void GateServer::tick(QWORD usec)
{
}

void GateServer::registerMgrs()
{
    addMgr(MgrDynamicFactory::PInstance());
    addMgr(MgrMessage::PInstance());
}

void GateServer::ForwardRpcMsg(const CMD::RpcMessage &message, RpcChannelPtr rpcChannelPtr)
{
    if (message.from() == ENUM::ESERVERTYPE_CLIENT 
        && !MgrUser::Instance().CheckUserRpcChannel(message.accid(), rpcChannelPtr))
    {
        return ;    // 检查客户端的账号是否和RpcChannel匹配
    }

    switch (message.to())
    {
    case ENUM::ESERVERTYPE_GAMESERVER:
        m_Gate2Game.Send(message);
        break;
    case ENUM::ESERVERTYPE_CLIENT:
        break;
    default:
        break;
    }
}
