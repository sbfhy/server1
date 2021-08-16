#pragma once

#include "muduo/base/common/condition.h"
#include "muduo/base/common/mutex.h"
#include "muduo/base/common/thread.h"
#include <string>

using std::string;

namespace muduo
{
namespace net
{

class EventLoop;

class EventLoopThread : private noncopyable
{
public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), const string& name = string());
  virtual ~EventLoopThread();

  EventLoop* startLoop();

private:
  void threadFunc();

private:
  EventLoop* m_loop GUARDED_BY(m_mutex);
  bool m_exiting;                                       // 
  Thread m_thread;                                      // 
  MutexLock m_mutex;                                    // 
  Condition m_cond GUARDED_BY(m_mutex);                 // 
  ThreadInitCallback m_callback;                        // 
};

}  // namespace net
}  // namespace muduo

