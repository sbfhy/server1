#pragma once

#include "src/base/server/server.h"

class GateServer : public Server 
{
public:
    GateServer(const ServerArgs& args) : Server(args) {}

private:
    virtual void tick(QWORD usec) override;
    virtual void registerMgrs() override;
};