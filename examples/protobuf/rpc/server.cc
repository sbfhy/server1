#include "message/test/sudoku.pb.h"

#include "muduo/base/common/logging.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/protorpc/RpcServer.h"
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

namespace sudoku
{

class SudokuServiceImpl : public SudokuService
{
public:
  virtual void Solve(const ::sudoku::SudokuRequestPtr& request,
                     const ::sudoku::SudokuResponsePtr& response)
  {
    LOG_INFO << "SudokuServiceImpl::Solve";
    if (!response) return ;
    response->set_solved(true);
    response->set_checkerboard("1234567");
    // done->Run();
  }
};

}

int main()
{
  LOG_INFO << "pid = " << getpid();
  EventLoop loop;
  InetAddress listenAddr(9981);
  RpcServer server(&loop, listenAddr);
//   server.registerService(std::make_shared<sudoku::SudokuServiceImpl>());
  server.start();
  loop.Loop();
}

