#pragma once

#include "muduo/base/common/noncopyable.h"
#include "muduo/base/common/condition.h"
#include "muduo/base/common/mutex.h"
#include "muduo/base/define/define_variable.h"

namespace muduo
{

/*
 * 1. 倒数门闩。用来等待其他线程都执行完任务，必要时可以对各个任务的执行结果进行汇总，然后主线程才继续往下执行。
 * 2. 传入一个整数，在这个整数“倒数”到0之前，主线程需要等待在门口，
 *    而这个“倒数”过程则是由各个执行线程驱动的，每个线程执行完一个任务“倒数”一次。
 * 3. 适合用于对任务进行拆分，使其并行执行，比如5个子任务交由5个线程并行执行，总的执行时间将决定于执行最慢的任务。
 * 4. 需要使用某些外部链接请求数据时，比如批量获取图片，也可以用CountDownLatch拆分给多个线程并行处理。
 */
class CountDownLatch : private noncopyable
{
public:
  explicit CountDownLatch(SDWORD count);

  /*
   * wait()使调用这个函数的线程处于等待状态(除非m_count==0)，一般是主线程调用。
   * 并没有规定只能有一个线程执行该方法，如果多个线程同时执行wait()方法，那么这几个线程都将处于等待状态，并且以共享模式享有同一个锁。
   */
  void wait();  
  void countDown();                                     // 计数减一
  SDWORD getCount() const; 

private:
  mutable MutexLock m_mutex;                            // 互斥量
  Condition m_condition GUARDED_BY(m_mutex);            // 条件变量
  SDWORD m_count GUARDED_BY(m_mutex);                     // 计数
};

} // namespace muduo

