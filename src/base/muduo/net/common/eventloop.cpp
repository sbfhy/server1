#include "muduo/net/common/eventloop.h"
#include "muduo/base/common/logging.h"
#include "muduo/base/common/mutex.h"
#include "muduo/base/define/define_new.h"
#include "muduo/net/common/channel.h"
#include "muduo/net/common/poller.h"
#include "muduo/net/common/timer_queue.h"
#include "muduo/net/common/sockets_ops.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

// namespace 
// {
__thread EventLoop* t_loopInThisThread = 0;               // 记录这个线程的EventLoop

extern const SDWORD kPollTimeMs = 10000;

SDWORD createEventfd()                                      // 创建eventfd, 事件通知文件描述符
{
  SDWORD evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);  // 创建一个能被用户应用程序用于时间等待唤醒机制的eventfd对象.
  if (evtfd < 0)
  {
    LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe
{
public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
  }
};
#pragma GCC diagnostic error "-Wold-style-cast"

IgnoreSigPipe initObj;
// } // namespace 


EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
  return t_loopInThisThread;
}

EventLoop::EventLoop()
  : m_looping(false)
  , m_quit(false)
  , m_eventHandling(false)
  , m_iteration(0)
  , m_poller(Poller::newDefaultPoller(this))
  , m_currentActiveChannel(nullptr)
  , m_callingPendingFunctors(false)
  , m_threadId(CurrentThread::tid())
  , m_timerQueue(NEW TimerQueue(this))
  , m_wakeupFd(createEventfd())
  , m_wakeupChannel(NEW Channel(this, m_wakeupFd))
{
  LOG_DEBUG << "EventLoop created " << static_cast<void*>(this);
  if (t_loopInThisThread)                               // 一个线程最多只能有一个EventLoop
  {
    LOG_FATAL << "Another EventLoop " << t_loopInThisThread << " exists in this thread " << m_threadId;
  }
  else
  {
    t_loopInThisThread = this;
  }
  m_wakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead, this));
  m_wakeupChannel->EnableReading();
}

EventLoop::~EventLoop()
{
  LOG_DEBUG << "EventLoop " << this << " of thread " << m_threadId << " destructs in thread " << CurrentThread::tid();
  m_wakeupChannel->DisableAll();
  m_wakeupChannel->Remove();
  ::close(m_wakeupFd);
  t_loopInThisThread = nullptr;
}

void EventLoop::Loop()
{
  assert(!m_looping);
  AssertInLoopThread();
  m_looping = true;
  m_quit = false;  // FIXME: what if someone calls quit() before loop() ?
  LOG_TRACE << "EventLoop " << this << " start looping";

  while (!m_quit)
  {
    m_activeChannels.clear();
    m_pollReturnTime = m_poller->poll(kPollTimeMs, &m_activeChannels);
    ++ m_iteration;
    if (Logger::getLogLevel() <= Logger::TRACE)
    {
      printActiveChannels();
    }
    // TODO sort channel by priority
    m_eventHandling = true;
    for (Channel* channel : m_activeChannels)
    {
      m_currentActiveChannel = channel;
      m_currentActiveChannel->HandleEvent(m_pollReturnTime);
    }
    m_currentActiveChannel = nullptr;
    m_eventHandling = false;
    doPendingFunctors();
  }
  
  LOG_TRACE << "EventLoop " << this << " stop looping";
  m_looping = false;
}

void EventLoop::Quit()
{
  m_quit = true;
  /*
   * There is a chance that loop() just executes while(!quit_) and exits, 
   * then EventLoop destructs, then we are accessing an invalid object.
   * Can be fixed using mutex_ in both places.
   */
  if (!IsInLoopThread())
  {
    Wakeup();
  }
}

void EventLoop::RunInLoop(Functor cb)
{
  if (IsInLoopThread())
  {
    cb();
  }
  else
  {
    QueueInLoop(std::move(cb));
  }
}

void EventLoop::QueueInLoop(Functor cb)
{
  {
  MutexLockGuard lock(m_mutex);
  m_pendingFunctors.push_back(std::move(cb));
  }
  if (!IsInLoopThread() || m_callingPendingFunctors)
  {
    Wakeup();
  }
}

size_t EventLoop::getQueueSize() const 
{
  MutexLockGuard lock(m_mutex);
  return m_pendingFunctors.size();
}

TimerId EventLoop::RunAt(TimeStamp time, TimerCallback cb)
{
  return m_timerQueue->AddTimer(std::move(cb), time, 0.0);
}

TimerId EventLoop::RunAfter(DOUBLE delay, TimerCallback cb)
{
  TimeStamp time(addTimeSeconds(TimeStamp::now(), delay));
  return RunAt(time, std::move(cb));
}

TimerId EventLoop::RunEvery(DOUBLE interval, TimerCallback cb)
{
  TimeStamp time(addTimeSeconds(TimeStamp::now(), interval));
  return m_timerQueue->AddTimer(std::move(cb), time, interval);
}

void EventLoop::Cancel(TimerId timerId)
{
  return m_timerQueue->Cancel(timerId);
}

void EventLoop::UpdateChannel(Channel* channel)
{
  assert(channel->getOwnerLoop() == this);
  AssertInLoopThread();
  m_poller->UpdateChannel(channel);
}

void EventLoop::RemoveChannel(Channel* channel)
{
  assert(channel->getOwnerLoop() == this);
  AssertInLoopThread();
  if (m_eventHandling)
  {
    assert(m_currentActiveChannel == channel 
           || std::find(m_activeChannels.begin(), m_activeChannels.end(), channel) == m_activeChannels.end());
  }
  m_poller->RemoveChannel(channel);
}

bool EventLoop::HasChannel(Channel* channel)
{
  assert(channel->getOwnerLoop() == this);
  AssertInLoopThread();
  return m_poller->HasChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in m_threadId = " << m_threadId
            << ", current thread id = " << CurrentThread::tid();
}

void EventLoop::Wakeup()
{
  QWORD one = 1;
  ssize_t n = sockets::write(m_wakeupFd, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::Wakeup() writes " << n << " bytes instead of 8";
  }
}

void EventLoop::handleRead()
{
  QWORD one = 1;
  ssize_t n = sockets::read(m_wakeupFd, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
  }
}

void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  m_callingPendingFunctors = true;
  
  {
  MutexLockGuard lock(m_mutex);
  functors.swap(m_pendingFunctors);
  }

  for (const Functor& functor : functors)
  {
    functor();
  }
  m_callingPendingFunctors = false;
}

void EventLoop::printActiveChannels() const 
{
  for (const Channel* channel : m_activeChannels)
  {
    LOG_TRACE << "{" << channel->ReventsToString() << "} ";
  }
}

