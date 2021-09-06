#include "src/base/server/server.h"

#include "muduo/base/common/logging.h"
#include "muduo/base/common/time_stamp.h"
#include "muduo/net/protorpc/RpcServer.h"
#include "muduo/net/common/inet_address.h"
#include "muduo/net/common/poller.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/channel.h"
#include "src/base/mgr/mgr_message.h"
#include "src/base/mgr/mgr_base.h"

// namespace {

/* extern const SDWORD kPollTimeMs; */
const SDWORD kPollTimeMs_ = 10;

// }   // namespace

Server::Server(const ServerArgs& args)
    : m_args(args)
{

}

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
        m_activeChannels.clear();
        m_pollReturnTime = m_poller->poll(kPollTimeMs_, &m_activeChannels);
        ++ m_iteration;
        if (Logger::getLogLevel() <= Logger::TRACE)
        {
            printActiveChannels();
        }
        
        // TODO sort channel by priority
        m_eventHandling = true;
        for (Channel* channel : m_activeChannels)
        {
            m_currentActiveChannel = channel;
            m_currentActiveChannel->HandleEvent(m_pollReturnTime);
        }
        m_currentActiveChannel = nullptr;
        m_eventHandling = false;
        
        usec = TimeStamp::now().getUSec();
        doPendingFunctors();
        tick(usec);
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
    registerSignal();
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
    wake();
    init();
    m_threadPool.start(1);
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
    InetAddress listenAddr(m_args.port);
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

void sig_handler(int sig_no)
{
    LOG_DEBUG << "signal : " << sig_no;
    Logger::LogFlush();
    exit(1);
}

void Server::registerSignal()
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));                   // 初始化信号结构
    act.sa_handler = sig_handler;                   // 设置信号处理函数

    if (sigaction(SIGINT, &act, NULL) < 0) {        // 注册 SIGINT 信号
        perror("sigaction");
        exit(1);
    }

    if (sigaction(SIGQUIT, &act, NULL) < 0) {       // 注册 SIGQUIT 信号
        perror("sigaction");
        exit(1);
    }

}
