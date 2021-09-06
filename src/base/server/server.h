#pragma once

#include "muduo/base/common/threadpool.h"
#include "muduo/net/common/eventloop.h"
#include "server_args.h"

#include<memory>

using namespace muduo;
using namespace muduo::net;

class MgrBase;

class Server : public EventLoop
             , public std::enable_shared_from_this<Server>
{
public:
    Server(const ServerArgs& args);

    virtual void Loop() override;
    virtual void Start();

    void Run();

protected:
    void addMgr(MgrBase* mgr);

private:
    virtual void tick(QWORD usec);
    virtual void registerMgrs() {}

    void registerSignal();
    void wake();
    void init();
    void listen();

private:
    ThreadPool m_threadPool;
    std::vector<MgrBase*> m_mgrs;
    DWORD m_warnFrameOffset = 50 * 1000;    // 帧耗时警告(微秒)
    ServerArgs m_args;
};
