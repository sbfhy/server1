#include "muduo/net/common/poller.h"
#include "muduo/net/common/channel.h"

using namespace muduo;
using namespace muduo::net;

Poller::Poller(EventLoop* loop)
  : m_ownerLoop(loop)
{
}

Poller::~Poller() = default;

bool Poller::HasChannel(Channel* channel) const 
{
  AssertInLoopThread();
  ChannelMap::const_iterator it = m_channels.find(channel->getFd());
  return it != m_channels.end() && it->second == channel;
}

