#include "message/test/sudoku.pb.h"

#include <muduo/base/common/logging.h>
#include <muduo/net/common/eventloop.h>
#include <muduo/net/common/inet_address.h>
#include <muduo/net/common/tcp_client.h>
#include <muduo/net/common/tcp_connection.h>
#include "muduo/net/protorpc/RpcChannel.h"

#include <unistd.h>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

class RpcClient : noncopyable
{
 public:
  RpcClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop),
      client_(loop, serverAddr, "RpcClient"),
      channel_(new RpcChannel),
      stub_(get_pointer(channel_))
  {
    client_.setConnectionCallback(
        std::bind(&RpcClient::onConnection, this, _1));
    client_.setMessageCallback(
        std::bind(&RpcChannel::onMessage, get_pointer(channel_), _1, _2, _3));
    // client_.enableRetry();
  }

  virtual ~RpcClient()
  {
    LOG_DEBUG << "loop地址: " << loop_;
  }

  void connect()
  {
    client_.connect();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    if (conn->IsConnected())
    {
      //channel_.reset(new RpcChannel(conn));
      channel_->setConnection(conn);
      sudoku::SudokuRequest request;
      request.set_checkerboard("001010");
    //   sudoku::SudokuResponse *response = new sudoku::SudokuResponse;

    //   stub_.Solve(nullptr, &request, response, NewCallback(this, &RpcClient::solved, response));
    }
    else
    {
      loop_->Quit();
    }
  }

  void solved(sudoku::SudokuResponse* resp)
  {
    LOG_INFO << "solved:\n" << resp->DebugString();
    client_.disconnect();
  }

  EventLoop* loop_;
  TcpClient client_;
  RpcChannelPtr channel_;
  sudoku::SudokuService::Stub stub_;
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 1)
  {
    LOG_INFO << "ip = " << argv[1];
    EventLoop loop;
    InetAddress serverAddr(argv[1], 9981);

    RpcClient rpcClient(&loop, serverAddr);
    rpcClient.connect();
    loop.Loop();
    google::protobuf::ShutdownProtobufLibrary();
  }
  else
  {
    printf("Usage: %s host_ip\n", argv[0]);
  }
}

