#include "muduo/net/common/poller.h"
#include "muduo/net/poller/poll_poller.h"
#include "muduo/net/poller/epoll_poller.h"
#include "muduo/base/define/define_new.h"
#include <stdlib.h>

using namespace muduo::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  if (::getenv("MUDUO_USE_POLL"))
  {
    return NEW PollPoller(loop);
  }
  else 
  {
    return NEW EPollPoller(loop);
  }
}

