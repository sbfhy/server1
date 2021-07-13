#include "muduo/base/common/threadpool.h"
#include "muduo/net/common/eventloop.h"

#include<memory>

using namespace muduo;
using namespace muduo::net;

class Server : public EventLoop
             , public std::enable_shared_from_this<Server>
{
public:
    virtual void Loop() override;
    virtual void Start();
    
    void Run();

private:
    virtual void tick() = 0;

    void listen();

private:
    ThreadPool m_threadPool;
};