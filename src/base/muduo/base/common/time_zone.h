#pragma once 

#include "muduo/base/common/copyable.h"
#include "define/define_variable.h"
#include <memory>
#include <time.h>

namespace muduo
{

// TimeZone for 1970~2030 
class TimeZone : public copyable
{
public:
  explicit TimeZone(const CHAR* zonefile);
  TimeZone(SDWORD eastOfUtc, const CHAR* tzname); // a fixed timezone
  TimeZone() = default;                         // an invalid timezone

  bool valid() const { return static_cast<bool>(m_data); }
  struct tm toLocalTime(time_t secondsSinceEpoch) const;  // 传入格林威治时间，转换成当地时间
  time_t fromLocalTime(const struct tm&) const;           // 传入当地时间，转换成格林威治时间

  // gmtime(3)
  static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);
  // timegm(3)
  static time_t fromUtcTime(const struct tm&);
  // year int [1900..2500], month in [1..12], day in [1..31] 
  static time_t fromUtcTime(SDWORD year, SDWORD month, SDWORD day, SDWORD hour, SDWORD minute, SDWORD seconds);

  struct TimeZoneData;

private:
  std::shared_ptr<TimeZoneData> m_data;
};

} // namespace muduo

