#pragma once

#include "src/base/server/server.h"

class GameServer : public Server 
{
public:

private:
    virtual void tick(QWORD usec) override;
    virtual void registerMgrs() override;
};