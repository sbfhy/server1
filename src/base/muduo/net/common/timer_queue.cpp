#include "muduo/net/common/timer_queue.h"
#include "muduo/base/common/logging.h"
#include "define/define_new.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/timer.h"
#include "muduo/net/common/timerid.h"
#include <algorithm>
#include <cstdint>
#include <ctime>
#include <sys/timerfd.h>
#include <unistd.h>
#include <vector>

namespace muduo
{
namespace net
{
namespace detail
{

SDWORD createTimerfd()
{
  SDWORD timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0)
  {
    LOG_SYSFATAL << "Failed in timerfd_create";
  }
  return timerfd;
}

struct timespec howMuchTimeFromNow(TimeStamp when)
{
  SQWORD microSeconds = when.getUSec() - TimeStamp::now().getUSec();
  if (microSeconds < 100)
  {
    microSeconds = 100;
  }
  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(microSeconds / TimeStamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<INTPTR>( (microSeconds % TimeStamp::kMicroSecondsPerSecond) * 1000 );
  return ts;
}

void readTimerfd(SDWORD timerfd, TimeStamp now)
{
  UINTPTR howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
  if (n != sizeof howmany)
  {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
  }
}

void resetTimerfd(SDWORD timerfd, TimeStamp expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  memZero(&newValue, sizeof newValue);
  memZero(&oldValue, sizeof oldValue);
  newValue.it_value = howMuchTimeFromNow(expiration);
  SDWORD ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret)
  {
    LOG_SYSERR << "timerfd_settime()";
  }
}

} // namespace detail
} // namespace net
} // namespace muduo

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : m_loop(loop)
  , m_timerfd(createTimerfd())
  , m_timerfdChannel(loop, m_timerfd)
  , m_timers()
  , m_callingExpiredTimers(false)
{
  m_timerfdChannel.setReadCallback(std::bind(&TimerQueue::handleRead, this));
  // we are always reading the timerfd, we disarm it with timerfd_settime.
  m_timerfdChannel.EnableReading();
}

TimerQueue::~TimerQueue()
{
  m_timerfdChannel.DisableAll();
  m_timerfdChannel.Remove();
  ::close(m_timerfd);
  // do not remove channel, since we're in EventLoop::dtor(); 
  for (const Entry& timer : m_timers)
  {
    delete(timer.second);
  }
}

TimerId TimerQueue::AddTimer(TimerCallback cb, TimeStamp when, DOUBLE interval)
{
  Timer* timer = NEW Timer(std::move(cb), when, interval);
  m_loop->RunInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer, timer->getSequence());
}

void TimerQueue::Cancel(TimerId timerId)
{
  m_loop->RunInLoop(std::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
  m_loop->AssertInLoopThread();
  bool earliestChanged = insert(timer);
  if (earliestChanged)
  {
    resetTimerfd(m_timerfd, timer->getExpiration());
  }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
  m_loop->AssertInLoopThread();
  assert(m_timers.size() == m_activeTimers.size());
  ActiveTimer timer(timerId.m_timer, timerId.m_sequence);
  ActiveTimerSet::iterator it = m_activeTimers.find(timer);
  if (it != m_activeTimers.end())
  {
    size_t n = m_timers.erase(Entry(it->first->getExpiration(), it->first));
    assert(n == 1);  (void)n;
    delete(it->first);   // FIXME: no delete please
    m_activeTimers.erase(it);
  }
  else if (m_callingExpiredTimers)
  {
    m_cancelingTimers.insert(timer);
  }
  assert(m_timers.size() == m_activeTimers.size());
}

void TimerQueue::handleRead()
{
  m_loop->AssertInLoopThread();
  TimeStamp now(TimeStamp::now());
  readTimerfd(m_timerfd, now);

  std::vector<Entry> expired = getExpired(now);
  m_callingExpiredTimers = true;
  m_cancelingTimers.clear();
  // safe to callback outside critical section
  for (const Entry& it : expired)
  {
    it.second->run();
  }
  m_callingExpiredTimers = false;
  reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(TimeStamp now)
{
  assert(m_timers.size() == m_activeTimers.size());
  std::vector<Entry> expired;
  Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
  TimerList::iterator end = m_timers.lower_bound(sentry);
  assert(end == m_timers.end() || now < end->first);
  std::copy(m_timers.begin(), end, back_inserter(expired));
  m_timers.erase(m_timers.begin(), end);

  for (const Entry& it : expired)
  {
    ActiveTimer timer(it.second, it.second->getSequence());
    size_t n = m_activeTimers.erase(timer);
    assert(n == 1);   (void)n;
  }
  assert(m_timers.size() == m_activeTimers.size());
  return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, TimeStamp now)
{
  TimeStamp nextExpire;
  for (const Entry& it : expired)
  {
    ActiveTimer timer(it.second, it.second->getSequence());
    if (it.second->getRepeat()                              // 如果这个定时器是可重复作用的，且不删除，就重启这个定时器
        && m_cancelingTimers.find(timer) == m_cancelingTimers.end())
    {
      it.second->restart(now);
      insert(it.second);
    }
    else
    {
      // FIXME move to a free list
      delete(it.second);   // FIXME: no delete please
    }
  }

  if (!m_timers.empty())
  {
    nextExpire = m_timers.begin()->second->getExpiration();
  }
  if (nextExpire.valid())
  {
    resetTimerfd(m_timerfd, nextExpire);
  }
}

bool TimerQueue::insert(Timer* timer)
{
  m_loop->AssertInLoopThread();
  assert(m_timers.size() == m_activeTimers.size());
  bool earliestChanged = false;
  TimeStamp when = timer->getExpiration();
  TimerList::iterator it = m_timers.begin();
  if (it == m_timers.end() || when < it->first)
  {
    earliestChanged = true;
  }
  {
    std::pair<TimerList::iterator, bool> result = m_timers.insert(Entry(when, timer));
    assert(result.second);  (void)result;
  }
  {
    std::pair<ActiveTimerSet::iterator, bool> result = m_activeTimers.insert(ActiveTimer(timer, timer->getSequence()));
    assert(result.second);  (void)result;
  }
  assert(m_timers.size() == m_activeTimers.size());
  return earliestChanged;
}

