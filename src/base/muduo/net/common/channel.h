#pragma once

#include "muduo/base/common/time_stamp.h"
#include "muduo/base/common/noncopyable.h"
#include <functional>
#include <memory>

namespace muduo
{
namespace net
{

class EventLoop;

/*
 * A selectable I/O channel.
 * This class doesn't own the file descriptor.  // Channel并不拥有文件描述符
 * The file descriptor could be a socket, an eventfd, a timerfd, or a signalfd.
 * 每个Channel只属于一个EventLoop，即只属于某一个IO线程。
 * Channel的成员函数都只能在IO线程调用，因此更新数据成员都不必加锁。
 */
class Channel : private noncopyable
{
public:
  typedef std::function<void()> EventCallback;
  typedef std::function<void(TimeStamp)> ReadEventCallback;

  Channel(EventLoop* loop, SDWORD fd);
  virtual ~Channel();

  void HandleEvent(TimeStamp receiveTime);  // 核心，处理事件接口，会调用handleEventWithGuard(1)，最后调用对应的回调函数
  void setReadCallback(ReadEventCallback cb) { m_readCallback = std::move(cb); }
  void setWriteCallback(EventCallback cb) { m_writeCallback = std::move(cb); }
  void setCloseCallback(EventCallback cb) { m_closeCallback = std::move(cb); }
  void setErrorCallback(EventCallback cb) { m_errorCallback = std::move(cb); }

  /*
   * Tie this channel to the owner object managed by shared_ptr, prevent the owner object being destroyed in handleEvent.
   * 用m_tie记录所有者(TcpConnection)的弱引用
   */
  void Tie(const std::shared_ptr<void>&);

  SDWORD getFd() const { return m_fd; }
  SDWORD getEvents() const { return m_events; }
  void setRevents(SDWORD revt) { m_revents = revt; } // used by pollers
  bool IsNoneEvent() const { return m_events == kNoneEvent; }

  /* 
   * 操作掩码m_events，即设置fd关心的事件(EPOLLIN/EPOLLOUT等)，并更新到poller中，最后会走到Poller::UpdateChannel。
   * Enable的时候，如果没有在poller中，会添加到poller中去；Disable的时候，如果事件为空，则会从poller中删除。
   */
  void EnableReading() { m_events |= kReadEvent; update(); }
  void DisableReading() { m_events &= ~kReadEvent; update(); }
  void EnableWriting() { m_events |= kWriteEvent; update();  }
  void DisableWriting() { m_events &= ~kWriteEvent; update(); }
  void EnableET() { m_events |= kEPollET; update(); }
  void DisableAll() { m_events = kNoneEvent; update(); }          // 从poller中删除
  bool IsWriting() const { return m_events & kWriteEvent; }
  bool IsReading() const { return m_events & kReadEvent; }

  // for Poller
  SDWORD getIndex() { return m_index; }
  void setIndex(SDWORD idx) { m_index = idx; }

  // for debug
  std::string ReventsToString() const;                      // 返回eventsToString(m_fd, m_revents)
  std::string EventsToString() const;                       // 返回eventsToString(m_fd, m_events)

  void DoNotLogHup() { m_logHup = false; }
  EventLoop* getOwnerLoop() { return m_loop; }
  void Remove();                                            // 从EventLoop中删除

private:
  static std::string eventsToString(SDWORD fd, SDWORD ev);      // for debug, 获取ev的所有掩码，返回字符串

  void update();                                            // 更新，最终会调用到poller::updateChannel(this)
  void handleEventWithGuard(TimeStamp receiveTime);         // 根据m_revents的激活类型，执行对应的回调函数

private:
  // 事件掩码
  static const SDWORD kNoneEvent;                             // 没有事件
  static const SDWORD kReadEvent;                             // 读事件，POLLIN | POLLPRI
  static const SDWORD kWriteEvent;                            // 写事件，POLLOUT
  static const SDWORD kEPollET;                               // 设置ET模式

  EventLoop* m_loop;
  const SDWORD m_fd;                                          // 文件描述符
  SDWORD m_events;                                            // 事件掩码，表示fd关心的事件。由用户来设置这个域，会更新到poller里去
  SDWORD m_revents; // it's the received event types of epoll or poll // 事件操作结果掩码，是目前活动的事件，由poller设置
  SDWORD m_index; // used by Poller.                          // 这个Channel在poller中的状态(kNew=-1, kAdded=1, kDeleted=2)
  bool m_logHup;                                            // 是否打印POLLHUP类型事件的日志

  std::weak_ptr<void> m_tie;          // 对所有者(TcpConnection)的弱引用，用于在handleEvent(1)时判断所有者是否还在
  bool m_tied;                                              // 是否设置了m_tie
  bool m_eventHandling;                                     // 是否正在处理事件，即在handleEventWithGuard(1)中的回调正在运行
  bool m_addedToLoop;                                       // 这个Channel是否添加到了EventLoop中

  ReadEventCallback m_readCallback;                         // 读回调，
  EventCallback m_writeCallback;                            // 写回调，
  EventCallback m_closeCallback;                            // 连接关闭回调，
  EventCallback m_errorCallback;                            // 报错回调，
};

} // namespace net
} // namespace muduo

