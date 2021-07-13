#pragma once

#include "muduo/base/common/copyable.h"
#include "muduo/base/define/define_variable.h"
#include <string>
#include <cmath>

namespace muduo
{

/*
 * Time stamp in UTC, in microseconds resolution.
 * 值类型
 */
class TimeStamp : public copyable
{
public:
  TimeStamp()
    : m_microSecondsSinceEpoch(0) {}
  explicit TimeStamp(QWORD microSecondsSinceEpochArg)
    : m_microSecondsSinceEpoch(microSecondsSinceEpochArg) {}

  void swap(TimeStamp& that) {
    std::swap(m_microSecondsSinceEpoch, that.m_microSecondsSinceEpoch);
  }
  std::string toString() const;  // 把m_microSecondsSinceEpoch转成("%" PRIu64 ".%06" PRIu64 "", sec, uSec)格式的字符串
  std::string toFormattedString(bool bShowUSec = true) const;// 把m_microSecondsSinceEpoch转成20201018 06:01:37.108001格式的字符串
  bool valid() const { return m_microSecondsSinceEpoch > 0; } // 是否有效
  QWORD getUSec() const { return m_microSecondsSinceEpoch; } // 微秒
  time_t getSec() const {                                     // 秒
    return static_cast<time_t>(m_microSecondsSinceEpoch / kMicroSecondsPerSecond); 
  }

  static TimeStamp now();                                     // 当前时间戳(微秒), 调用gettimeofday(2),不是系统调用，不会陷入内核
  static TimeStamp invalid() { return TimeStamp(); }          // 无效时间戳，0
  static TimeStamp fromUnixTime(time_t t) { return fromUnixTime(t, 0); }  // 将time_t格式的时间戳转换成TimeStamp格式
  static TimeStamp fromUnixTime(time_t t, SDWORD microSeconds) {            // 
    return TimeStamp(static_cast<QWORD>(t) * kMicroSecondsPerSecond + microSeconds);
  }

  static const SDWORD kMicroSecondsPerSecond = 1000 * 1000;     // 常量，一秒等于多少微秒

private:
  QWORD m_microSecondsSinceEpoch;                            // 从1970年开始的微秒数
};

inline bool operator<(TimeStamp lhs, TimeStamp rhs)
{
  return lhs.getUSec() < rhs.getUSec();
}

inline bool operator==(TimeStamp lhs, TimeStamp rhs)
{
  return lhs.getUSec() == rhs.getUSec();
}

/*
 * Gets time difference of two timestamps, result in seconds.
 * double has 52-bit precision, enough for one-microsecond resolution for next 100 years.
 */
inline DOUBLE getDiffSeconds(TimeStamp t1, TimeStamp t2)
{
  SQWORD diff = std::abs(SQWORD(t1.getUSec() - t2.getUSec()));
  return static_cast<DOUBLE>(diff) / TimeStamp::kMicroSecondsPerSecond;
}

// 返回timeStamp + seconds 的时间戳
inline TimeStamp addTimeSeconds(TimeStamp timeStamp, DOUBLE seconds)
{
  QWORD delta = static_cast<QWORD>(seconds * TimeStamp::kMicroSecondsPerSecond);
  return TimeStamp(timeStamp.getUSec() + delta);
}

} // namespace muduo



