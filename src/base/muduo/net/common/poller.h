#pragma once

#include "muduo/base/common/time_stamp.h"
#include "muduo/net/common/eventloop.h"
#include <map>
#include <vector>

namespace muduo
{
namespace net
{

class Channel;

/*
 * Base class for IO Multiplexing.
 * This class doesn't own the Channel objects.  // 没有包含Channel对象
 * 负责IO multiplexing，它不负责事件分发，事件分发由Channel::HandleEvent负责。
 */
class Poller : private noncopyable
{
public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
  virtual ~Poller();

  // Polls the I/O events. Must be called in the loop thread. 
  virtual TimeStamp poll(SDWORD timeoutMs, ChannelList* activeChannels) = 0;

  // Changes the interested I/O events. Must be called in the loop thread.
  virtual void UpdateChannel(Channel* channel) = 0; // 负责维护和更新m_channels

  // Remove the channel, when it destructs. Must be called in the loop thread.
  // poller并不拥有Channel，Channel在析构之前必须调用这个函数unregister，避免空悬指针
  virtual void RemoveChannel(Channel* channel) = 0;

  virtual bool HasChannel(Channel* channel) const;
  
  void AssertInLoopThread() const { m_ownerLoop->AssertInLoopThread(); }

  static Poller* newDefaultPoller(EventLoop* loop);

protected:
  typedef std::map<SDWORD, Channel*> ChannelMap;
  ChannelMap m_channels;                                // <fd, Channel*>

private:
  EventLoop* m_ownerLoop;
};

} // namespace net
} // namespace muduo

