#include "message/test/sudoku.pb.h"

#include <muduo/base/common/logging.h>
#include "muduo/base/common/threadpool.h"
#include <muduo/net/common/eventloop.h>
#include <muduo/net/protorpc/RpcServer.h>
#include "src/base/mgr/mgr_message.h"

#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class TestServer
{
public:
  TestServer(std::shared_ptr<EventLoop> mainLoop) 
  {
    m_mainLoop = mainLoop;
  }

  void Start()
  {
    m_threadPool.start(1);
    m_threadPool.run(std::bind(&TestServer::listen, this));
  }

private:
  void listen()
  {
    EventLoop loop;
    InetAddress listenAddr(9981);
    RpcServer server(m_mainLoop, &loop, listenAddr);
    server.setThreadNum(1);
    // server.registerService(std::make_shared<sudoku::SudokuServiceImpl>());
    server.start();
    loop.Loop();
  }

private:
  ThreadPool m_threadPool;
  std::shared_ptr<EventLoop> m_mainLoop;
};

int main()
{
  LOG_INFO << "pid = " << getpid();

  MgrMessage::Instance().Wake();

  std::shared_ptr<EventLoop> mainLoopPtr = std::make_shared<EventLoop>();

  TestServer testServer(mainLoopPtr);
  testServer.Start();
  
  mainLoopPtr->Loop();

  return 0;
}

