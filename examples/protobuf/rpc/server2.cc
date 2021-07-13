#include <sudoku.pb.h>

#include <muduo/base/common/logging.h>
#include "muduo/base/common/threadpool.h"
#include <muduo/net/common/eventloop.h>
#include <muduo/net/protorpc/RpcServer.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

namespace sudoku
{

class SudokuServiceImpl : public SudokuService
{
public:
  virtual void Solve(::google::protobuf::RpcController* controller,
                     const ::sudoku::SudokuRequest* request,
                     ::sudoku::SudokuResponse* response,
                     ::google::protobuf::Closure* done)
  {
    LOG_INFO << "SudokuServiceImpl::Solve";
    response->set_solved(true);
    response->set_checkerboard("1234567");
    done->Run();
  }
};

}

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
    sudoku::SudokuServiceImpl impl;
    RpcServer server(m_mainLoop, &loop, listenAddr);
    server.setThreadNum(1);
    server.registerService(&impl);
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

  std::shared_ptr<EventLoop> mainLoopPtr = std::make_shared<EventLoop>();

  TestServer testServer(mainLoopPtr);
  testServer.Start();
  
  mainLoopPtr->Loop();
}

