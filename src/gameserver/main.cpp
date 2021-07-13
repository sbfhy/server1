#include "src/gameserver/gameserver.h"

int main(int argc, char* argv[])
{
    GameServer server;
    server.Start();
    server.Run();

    return 0;
}
