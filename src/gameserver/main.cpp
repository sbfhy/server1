#include "src/gameserver/gameserver.h"

#include "muduo/base/common/logging.h"

#include <memory>

int main(int argc, char* argv[])
{
    Logger::SetLogging(argv[0], 200*1000);

    std::shared_ptr<GameServer> server = 
        std::make_shared<GameServer>(
            ServerArgs{9982}
        );
    server->Start();
    server->Run();

    return 0;
}
