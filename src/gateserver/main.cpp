#include "src/gateserver/gateserver.h"
#include "muduo/base/common/logging.h"

#include <memory>

int main(int argc, char* argv[])
{
    Logger::SetLogging(argv[0], 200*1000);

    std::shared_ptr<GateServer> server = 
        std::make_shared<GateServer>( 
            ServerArgs{9981}
        );
    server->Start();
    server->Run();

    return 0;
}
