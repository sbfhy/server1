#include "muduo/base/common/time_stamp.h"
#include <cstdio>
#include <sys/time.h>
#include <inttypes.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS                            // PRID64, PRIu64
#endif

using namespace muduo;

std::string TimeStamp::toString() const
{
  CHAR buf[32] = {0};
  QWORD sec = getUSec() / kMicroSecondsPerSecond;
  QWORD uSec = getUSec() % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf), "%" PRIu64" .%06" PRIu64" ", sec, uSec);
  return buf;
}

std::string TimeStamp::toFormattedString(bool bShowUSec) const 
{
  CHAR buf[64] = {0};
  time_t sec = static_cast<time_t>(getUSec() / kMicroSecondsPerSecond);
  struct tm tm_time;  // 系统的时间结构, 注意：tm里的年份是从1900年起至今多少年，而不是直接存储如2008年，月份从0开始的，0表示一月，星期也是从0开始的， 0表示星期日，1表示星期一。
  gmtime_r(&sec, &tm_time);         // 把time_t格式转换成tm格式
  if (bShowUSec)
  {
    SDWORD uSec = static_cast<SDWORD>(getUSec() % kMicroSecondsPerSecond);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d", 
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday, 
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, 
             uSec);
  }
  else
  {
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d", 
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday, 
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
}

TimeStamp TimeStamp::now()
{
  struct timeval tv;                                    // timeval系统时间格式，精确到微秒
  gettimeofday(&tv, nullptr);                           // 获取当前时间戳，微秒。不是系统调用，不会陷入内核 
  QWORD sec = tv.tv_sec;
  return TimeStamp(sec * kMicroSecondsPerSecond + tv.tv_usec);
}


