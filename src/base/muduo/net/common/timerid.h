#pragma once

#include "muduo/base/common/copyable.h"
#include "define/define_variable.h"

namespace muduo
{
namespace net
{

class Timer;

/*
 * An opaque identifier, for canceling Timer.
 * 包装了一下定时器，在TimerQueue里删除定时器用
 */
class TimerId : public copyable
{
public:
  TimerId()
    : m_timer(nullptr)
    , m_sequence(0) {}

  TimerId(Timer* timer, SQWORD seq)
    : m_timer(timer)
    , m_sequence(seq) {}

  // default copy-ctor, dtor and assignment are okay

  friend class TimerQueue;

private:
  Timer* m_timer;                                               // 
  SQWORD m_sequence;                                             // 该定时器序号
};

} // net
} // namespace muduo

