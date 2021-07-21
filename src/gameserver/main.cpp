#include "src/gameserver/gameserver.h"

#include <memory>

int main(int argc, char* argv[])
{
    std::shared_ptr<GameServer> server = std::make_shared<GameServer>();
    server->Start();
    server->Run();

    return 0;
}
