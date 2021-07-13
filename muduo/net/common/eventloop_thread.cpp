#include "muduo/net/common/eventloop_thread.h"
#include "muduo/net/common/eventloop.h"

using namespace muduo;
using namespace muduo::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const string& name)
  : m_loop(nullptr)
  , m_exiting(false)
  , m_thread(std::bind(&EventLoopThread::threadFunc, this), name)
  , m_mutex()
  , m_cond(m_mutex)
  , m_callback(cb)
{
}

EventLoopThread::~EventLoopThread()
{
  m_exiting = true;
  if (m_loop != nullptr) // not 100% race-free, eg. threadFunc could be running m_callback.
  {
    // still a tiny chance to call destructed object, if threadFunc exits just now.
    // but when EventLoopThread destructs, usually programming is exiting anyway.
    m_loop->Quit();
    m_thread.join();
  }
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!m_thread.started());
  m_thread.start();

  EventLoop* loop = nullptr;
  {
    MutexLockGuard lock(m_mutex);
    while (m_loop == nullptr)
    {
      m_cond.wait();
    }
    loop = m_loop;
  }

  return loop;
}

void EventLoopThread::threadFunc() 
{
  EventLoop loop;
  if (m_callback)
  {
    m_callback(&loop);
  }

  {
    MutexLockGuard lock(m_mutex);
    m_loop = &loop;
    m_cond.notify();
  }

  loop.Loop();
  //assert(m_exiting);
  MutexLockGuard lock(m_mutex);
  m_loop = nullptr;
}

