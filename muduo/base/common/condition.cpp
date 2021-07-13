#include "muduo/base/common/condition.h"
#include <ctime>

// returns true if time out, false otherwise.
bool muduo::Condition::waitForSeconds(double seconds)
{
  struct timespec abstime;
  // FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
  clock_gettime(CLOCK_REALTIME, &abstime);

  const SQWORD kNanoSecondsPerSecond = 1000000000;
  SQWORD nanoseconds = static_cast<SQWORD>(seconds * kNanoSecondsPerSecond);

  abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
  abstime.tv_nsec = static_cast<INTPTR>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

  MutexLock::UnassignGuard ug(m_mutex);
  return ETIMEDOUT == pthread_cond_timedwait(&m_pcond, m_mutex.getPthreadMutex(), &abstime);
}


