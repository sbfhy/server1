#include "muduo/base/common/date.h"
#include <stdio.h>        // snprintf
#include <time.h>         // struct tm

namespace muduo
{
namespace detail
{

CHAR require_32_bit_integer_at_least[sizeof(SDWORD) >= sizeof(int32_t) ? 1 : -1];

/*
 * algorithm and explanation see: 
 * http://www.faqs.org/faqs/calendars/faq/part2/
 * http://blog.csdn.net/Solstice
 * 公历时间格式 转换成 儒略日
 */
SDWORD getJulianDayNumber(SDWORD year, SDWORD month, SDWORD day)
{
  (void) require_32_bit_integer_at_least; // no warning please
  SDWORD a = (14 - month) / 12;
  SDWORD y = year + 4800 - a;
  SDWORD m = month + 12 * a - 3;
  return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
}

// 儒略日 转换成 公历时间
struct Date::YearMonthDay getYearMonthDay(SDWORD julianDayNumber)
{
  SDWORD a = julianDayNumber + 32044;
  SDWORD b = (4 * a + 3) / 146097;
  SDWORD c = a - ((b * 146097) / 4);
  SDWORD d = (4 * c + 3) / 1461;
  SDWORD e = c - ((1461 * d) / 4);
  SDWORD m = (5 * e + 2) / 153;
  Date::YearMonthDay ymd;
  ymd.day = e - ((153 * m + 2) / 5) + 1;
  ymd.month = m + 3 - 12 * (m / 10);
  ymd.year = b * 100 + d - 4800 + (m / 10);
  return ymd;
}
} // namespace detail
const SDWORD Date::kJulianDayOf19700101 = detail::getJulianDayNumber(1970, 1, 1);
} // namespace muduo

using namespace muduo;
using namespace muduo::detail;

Date::Date(SDWORD y, SDWORD m, SDWORD d)
  : m_julianDayNumber(getJulianDayNumber(y, m, d))
{
}

Date::Date(const struct tm& t)
  : m_julianDayNumber(getJulianDayNumber(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday))
{
}

std::string Date::toIsoString() const 
{
  CHAR buf[32];
  YearMonthDay ymd(yearMonthDay());
  snprintf(buf, sizeof buf, "%4d-%02d-%02d", ymd.year, ymd.month, ymd.day);
  return buf;
}

Date::YearMonthDay Date::yearMonthDay() const 
{
  return getYearMonthDay(m_julianDayNumber);
}

