#include "echo_server.h"
#include "muduo/base/common/logging.h"
#include "muduo/net/common/eventloop.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  
  muduo::net::EventLoop loop;
  muduo::net::InetAddress listenAddr(2007);
  EchoServer server(&loop, listenAddr);
  server.start();
  loop.Loop();

  return 0;
}
