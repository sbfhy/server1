#include "src/base/server/server.h"

#include "muduo/base/common/logging.h"
#include "muduo/net/protorpc/RpcServer.h"
#include "muduo/net/common/inet_address.h"
#include "muduo/base/common/time_stamp.h"
#include "src/base/mgr/mgr_message.h"
#include "src/base/mgr/mgr_base.h"

void Server::Loop()
{
    assert(!m_looping);
    AssertInLoopThread();
    m_looping = true;
    m_quit = false; // FIXME: what if someone calls quit() before loop() ?
    LOG_TRACE << "EventLoop " << this << " start looping";

    QWORD usec = 0;
    QWORD postUsec = TimeStamp::now().getUSec();
    DWORD frameOffset = 0;

    while (!m_quit)
    {
        ++m_iteration;
        usec = TimeStamp::now().getUSec();
        tick(usec);
        doPendingFunctors();
        postUsec = TimeStamp::now().getUSec();
        frameOffset = postUsec - usec;
        if (frameOffset > m_warnFrameOffset)
        {
            LOG_DEBUG << "[frame offset] " << frameOffset;
        }
    }

    LOG_TRACE << "EventLoop " << this << " stop looping";
    m_looping = false;
}

void Server::wake()
{
    registerMgrs();
    for (auto mgr : m_mgrs)
    {
        if (mgr)
        {
            mgr->Wake();
        }
    }
}

void Server::Start()
{
    m_threadPool.start(1);
    wake();
    init();
}

void Server::Run()
{
    m_threadPool.run(std::bind(&Server::listen, this));
    Loop();
}

void Server::tick(QWORD usec)
{
    for (auto mgr : m_mgrs)
    {
        if (mgr)
        {
            mgr->Tick(usec);
        }
    }
}

void Server::listen()
{
    EventLoop loop;
    InetAddress listenAddr(9981);
    RpcServer server(shared_from_this(), &loop, listenAddr);
    server.setThreadNum(1);
    server.start();
    loop.Loop();
}

void Server::init()
{
    for (auto mgr : m_mgrs)
    {
        if (mgr)
        {
            mgr->Init();
        }
    }
}

void Server::addMgr(MgrBase* mgr)
{
    m_mgrs.push_back(mgr);
}
