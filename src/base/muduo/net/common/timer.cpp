#include "muduo/net/common/timer.h"

using namespace muduo;
using namespace muduo::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(TimeStamp now)
{
  if (m_repeat)
  {
    m_expiration = addTimeSeconds(now, m_interval);
  }
  else
  {
    m_expiration = TimeStamp::invalid();
  }
}

