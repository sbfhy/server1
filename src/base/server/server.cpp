#include "src/base/server/server.h"
#include "muduo/base/common/logging.h"
#include "muduo/net/protorpc/RpcServer.h"
#include "muduo/net/common/inet_address.h"

void Server::Loop()
{
    assert(!m_looping);
    AssertInLoopThread();
    m_looping = true;
    m_quit = false; // FIXME: what if someone calls quit() before loop() ?
    LOG_TRACE << "EventLoop " << this << " start looping";

    while (!m_quit)
    {
        ++m_iteration;
        tick();
        doPendingFunctors();
    }

    LOG_TRACE << "EventLoop " << this << " stop looping";
    m_looping = false;
}

void Server::Start()
{
    m_threadPool.start(1);
}

void Server::Run()
{
    m_threadPool.run(std::bind(&Server::listen, this));
    Loop();
}

void Server::listen()
{
    EventLoop loop;
    InetAddress listenAddr(9981);
    // sudoku::SudokuServiceImpl impl;
    RpcServer server(shared_from_this(), &loop, listenAddr);
    server.setThreadNum(1);
    // server.registerService(&impl);
    server.start();
    loop.Loop();
}
