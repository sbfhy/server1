#pragma once

#include "muduo/base/common/mutex.h"
#include <pthread.h>

namespace muduo
{

/*
 * 封装条件变量，一般和互斥锁同时使用。
 * 线程同步，用来阻塞一个线程，直到条件满足被触发为止
 * 条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个/多个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"信号。
 */
class Condition : private noncopyable
{
public:
  explicit Condition(MutexLock& mutex)
    : m_mutex(mutex)
  {
    MCHECK(pthread_cond_init(&m_pcond, nullptr));       // 初始化
  }
  ~Condition()
  {
    MCHECK(pthread_cond_destroy(&m_pcond));             // 销毁
  }

  void wait()                                           // 阻塞当前线程
  {
    MutexLock::UnassignGuard ug(m_mutex);               // UnassignGuard在构造时取消持有，析构时再由当前线程持有
    /*
     * pthread_cond_wait()
     * 1. 用于阻塞当前线程，等待别的线程使用pthread_cond_signal()或pthread_cond_broadcast来唤醒它。 
     * 2. 必须与pthread_mutex配套使用, pthread_cond_wait()函数一进入wait状态就会自动release mutex，
     *    当被唤醒，pthread_cond_wait()通过（返回）时，该线程又自动获得该 mutex。
     */
    MCHECK(pthread_cond_wait(&m_pcond, m_mutex.getPthreadMutex()));
  }

  // returns true if time out, false otherwise.
  bool waitForSeconds(double seconds);                  // 阻塞当前线程多少秒

  void notify()                                         // 唤醒一个线程，如有多个线程等待，会有调试策略(一般根据线程优先级)
  {
    MCHECK(pthread_cond_signal(&m_pcond));
  }

  void notifyAll()                                      // 惊群现象，唤醒在等待这个条件变量的全部线程
  {
    MCHECK(pthread_cond_broadcast(&m_pcond));
  }

private:
  MutexLock&      m_mutex;                              // 互斥量
  pthread_cond_t  m_pcond;                              // 条件变量
};

} // namespace muduo

