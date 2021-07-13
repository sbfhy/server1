#include "muduo/base/common/count_down_latch.h"

using namespace muduo;

CountDownLatch::CountDownLatch(SDWORD count)
  : m_mutex()
  , m_condition(m_mutex)
  , m_count(count)
{
}

void CountDownLatch::wait()
{
  MutexLockGuard lock(m_mutex);
  while (m_count > 0)
  {
    m_condition.wait();
  }
}

void CountDownLatch::countDown()
{
  MutexLockGuard lock(m_mutex);
  -- m_count;
  if (m_count == 0)
  {
    m_condition.notifyAll();
  }
}

SDWORD CountDownLatch::getCount() const 
{
  MutexLockGuard lock(m_mutex);
  return m_count;
}

