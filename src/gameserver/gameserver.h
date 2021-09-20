#pragma once

#include "src/base/server/server.h"

namespace muduo {
namespace net {
    class RpcChannel;
}   // namespace muduo
}   // namespace net

namespace google {
namespace protobuf {

class Message;           // message.h
typedef ::std::shared_ptr<Message> MessagePtr;

} // namespace protobuf
} // namespace google


class GameServer : public Server 
{
public:
    GameServer(const ServerArgs& args); 

    virtual ENUM::EServerType GetServerType() const { return ENUM::ESERVERTYPE_GAMESERVER; }

    void SetGateRpcChannelPtr(std::shared_ptr<muduo::net::RpcChannel> ptr) { m_gateRpcChannelPtr = ptr; }

    void SendToGate(const ::google::protobuf::MessagePtr& request, QWORD accid = 0);

private:
    virtual void tick(QWORD usec) override;
    virtual void registerMgrs() override;

private:
    std::shared_ptr<muduo::net::RpcChannel> m_gateRpcChannelPtr;  // gateserver
};

extern std::shared_ptr<GameServer> thisServer;
