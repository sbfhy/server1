#pragma once

#include "muduo/net/common/poller.h"
#include <vector>

struct pollfd;

namespace muduo
{
namespace net
{
/*
 * IO Multiplexing with poll(2).
 */
class PollPoller : public Poller
{
public:
  PollPoller(EventLoop* loop);
  virtual ~PollPoller() override;

  virtual TimeStamp poll(SDWORD timeoutMs, ChannelList* activeChannels) override; // 调用::poll(3)
  virtual void UpdateChannel(Channel* channel) override;
  virtual void RemoveChannel(Channel* channel) override;

private:
  void fillActiveChannels(SDWORD numEvents, ChannelList* activeChannels) const; // O(n)复杂度
  
private:
  typedef std::vector<struct pollfd> PollFdList;
  PollFdList m_pollfds;
};

} // namespace net
} // namespace muduo

