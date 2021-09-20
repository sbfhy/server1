#pragma once

#include "src/base/server/server.h"

#include "muduo/net/common/rpc_client.h"

namespace CMD {
    class RpcMessage;
}   // namespace CMD


class GateServer : public Server 
{
public:
    GateServer(const ServerArgs& args);

    virtual void Start() override;
    virtual ENUM::EServerType GetServerType() const { return ENUM::ESERVERTYPE_GATESERVER; }

    virtual void ForwardRpcMsg(const CMD::RpcMessage &message, RpcChannelPtr rpcChannelPtr);

    void SendToGame(const ::google::protobuf::MessagePtr& request);

private:
    virtual void tick(QWORD usec) override;
    virtual void registerMgrs() override;
    
    void onConnectionSucceedCb();

private:
    RpcClient m_Gate2Game;
};

extern std::shared_ptr<GateServer> thisServer;
