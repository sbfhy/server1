#include "muduo/net/common/eventloop_thread_pool.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/eventloop_thread.h"
#include "define/define_types.h"
#include "define/define_new.h"
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg)
  : m_baseLoop(baseLoop)
  , m_name(nameArg)
  , m_started(false)
  , m_numThreads(0)
  , m_next(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
  // Don't delete loop, it's stack variable
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
  assert(!m_started);
  m_baseLoop->AssertInLoopThread();

  m_started = true;

  for (int i = 0; i < m_numThreads; ++i)                        // 新建出m_numThreads个线程，每个线程有一个EventLoop
  {
    char buf[m_name.size() + 32];
    snprintf(buf, sizeof buf, "%s%d", m_name.c_str(), i);
    EventLoopThread* t = NEW EventLoopThread(cb, buf);
    m_threads.push_back(std::unique_ptr<EventLoopThread>(t));
    m_loops.push_back(t->startLoop());
  }
  if (m_numThreads == 0 && cb)
  {
    cb(m_baseLoop);
  }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
  m_baseLoop->AssertInLoopThread();
  assert(m_started);
  EventLoop* loop = m_baseLoop;
  if (!m_loops.empty())
  {
    // round-robin
    loop = m_loops[m_next];
    ++m_next;
    if (implicit_cast<size_t>(m_next) >= m_loops.size())
    {
      m_next = 0;
    }
  }
  return loop;
}

EventLoop* EventLoopThreadPool::getLoopForHash(size_t hashCode)
{
  m_baseLoop->AssertInLoopThread();
  EventLoop* loop = m_baseLoop;
  if (!m_loops.empty())
  {
    loop = m_loops[hashCode % m_loops.size()];
  }
  return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
  m_baseLoop->AssertInLoopThread();
  assert(m_started);
  if (m_loops.empty())
  {
    return std::vector<EventLoop*>(1, m_baseLoop);
  }
  else
  {
    return m_loops;
  }
}
