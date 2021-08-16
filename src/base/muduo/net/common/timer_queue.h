#pragma once

#include "muduo/base/common/mutex.h"
#include "muduo/base/common/time_stamp.h"
#include "muduo/net/common/callbacks.h"
#include "muduo/net/common/channel.h"
#include <set>
#include <vector>

namespace muduo
{
namespace net
{

class EventLoop;
class Timer;
class TimerId;

/*
 * A best efforts timer queue.
 * No guarantee that the callback will be on time.  // 不能保证回调一定准时
 */
class TimerQueue : noncopyable
{
public:
  explicit TimerQueue(EventLoop* loop);
  virtual ~TimerQueue();

  /*
   * Schedules the callback to be run at given time, repeats if @c interval > 0.0.
   * Must be thread safe. Usually be called from other threads.
   */
  TimerId AddTimer(TimerCallback cb, TimeStamp when, DOUBLE interval);

  void Cancel(TimerId timerId);

private:
  // FIXME: use unique_ptr<Timer> instead of raw pointers. 
  // This requires heterogeneous comparison lookup (N3465) from C++14 so that we can find an T* in a set<unique_ptr<T>>.
  typedef std::pair<TimeStamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;
  typedef std::pair<Timer*, SQWORD> ActiveTimer;
  typedef std::set<ActiveTimer> ActiveTimerSet;

  void addTimerInLoop(Timer* timer);
  void cancelInLoop(TimerId timerId);
  void handleRead();  // called when timerfd alarms
  std::vector<Entry> getExpired(TimeStamp now); // move out all expired timers
  void reset(const std::vector<Entry>& expired, TimeStamp now);
  bool insert(Timer* timer);
 
private:
  EventLoop* m_loop;
  const SDWORD m_timerfd;
  Channel m_timerfdChannel;
  TimerList m_timers; // Timer list sorted by expiration  // 按截止时间排序
  
  // for cancel()
  ActiveTimerSet m_activeTimers;
  bool m_callingExpiredTimers; /* atomic */
  ActiveTimerSet m_cancelingTimers;
};

} // namespace net
} // namespace muduo

