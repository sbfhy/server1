#pragma once 

#include "muduo/base/common/atomic.h"
#include "muduo/base/common/time_stamp.h"
#include "muduo/net/common/callbacks.h"

namespace muduo
{
namespace net
{

class Timer : noncopyable
{
public:
  Timer(TimerCallback cb, TimeStamp when, DOUBLE interval)
    : m_callback(std::move(cb))
    , m_expiration(when)
    , m_interval(interval)
    , m_repeat(interval > 0.0)
    , m_sequence(s_numCreated_.incrementAndGet()) {}

  void run() const                                              // 
  {
    m_callback();
  }
  
  void restart(TimeStamp now);                                  // 重启，主要是重新设置一下m_expiration

  TimeStamp getExpiration() const { return m_expiration; }
  bool getRepeat() const { return m_repeat; }
  SQWORD getSequence() const { return m_sequence; }

  static SQWORD getNumCreated() { return s_numCreated_.get(); }

private:
  const TimerCallback m_callback;                               // 回调，std::function<void()>
  TimeStamp m_expiration;                                       // 截止时间戳
  const DOUBLE m_interval;                                        // 间隔
  const bool m_repeat;                                          // m_interval>0.0 为true，即每隔一段时间 restart()
  const SQWORD m_sequence;                                       // 标记ID，记录这个定时器是第几个创建的

  static AtomicInt64 s_numCreated_;                             // 创建的定时器数量
};

} // namespace net
} // namespace muduo

