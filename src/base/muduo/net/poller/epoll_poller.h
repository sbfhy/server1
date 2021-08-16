#pragma once

#include "muduo/net/common/poller.h"
#include <vector>

struct epoll_event;

namespace muduo
{
namespace net
{
/*
 * IO Multiplexing with epoll(4).
 */
class EPollPoller : public Poller
{
public:
  EPollPoller(EventLoop* loop);
  virtual ~EPollPoller() override;
  
  virtual TimeStamp poll(SDWORD timeoutMs, ChannelList* activeChannels) override; // 调用::epoll_wait(4)和fillActiveChannels(2)
  virtual void UpdateChannel(Channel* channel) override;
  virtual void RemoveChannel(Channel* channel) override;

private:
  void fillActiveChannels(SDWORD numEvents, ChannelList* activeChannels) const;  // 把需要处理的Channel存入到activeChannels中
  
  /*
   * operation 操作类型 : EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD
   * 将channel存到一个epoll_event中，然后调用::epoll_ctl(4)去操作这个epoll_event
   */
  void update(SDWORD operation, Channel* channel); 
  
  static const CHAR* operationToString(SDWORD op);        // 打日志用

private:
  typedef std::vector<struct epoll_event> EventList;

  SDWORD m_epollfd;                                       // epoll_create1()的返回值，用于管理这个epoll实例，即管理一些epoll_event集合
  EventList m_events;                                     // 存储::epoll_wait(4)接收到的epoll_event
  
  static const SDWORD kInitEventListSize = 16;            // 初始化m_events的大小
};

}  // namespace net
}  // namespace muduo
