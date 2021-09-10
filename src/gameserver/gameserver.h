#pragma once

#include "src/base/server/server.h"

class GameServer : public Server 
{
public:
    GameServer(const ServerArgs& args); 

    virtual ENUM::EServerType GetServerType() const { return ENUM::ESERVERTYPE_GAMESERVER; }

private:
    virtual void tick(QWORD usec) override;
    virtual void registerMgrs() override;
};

