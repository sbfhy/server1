#pragma once

#include "src/base/server/server.h"

#include "muduo/net/common/tcp_client.h"

class GameServer : public Server 
{
public:
    GameServer(const ServerArgs& args); 
    
    virtual void Start() override;

private:
    virtual void tick(QWORD usec) override;
    virtual void registerMgrs() override;

private:
    TcpClient m_game2gate;
};
