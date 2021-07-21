#include <sudoku.pb.h>

#include <muduo/base/common/logging.h>
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

