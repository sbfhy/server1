#pragma once 

#include "muduo/base/common/copyable.h"
#include "define/define_types.h"
#include "define/define_variable.h"

#include <string>

struct tm;

namespace muduo
{

/*
 * 公历时间
 * 建议只当值类型传递
 */
class Date : public copyable
{
public:
  struct YearMonthDay
  {
    SDWORD year;                                          // [1900..2500]
    SDWORD month;                                         // [1..12]
    SDWORD day;                                           // [1..31]
  };

  static const SDWORD kDaysPerWeek = 7;
  static const SDWORD kJulianDayOf19700101;               // 儒略日19700101

  // 构造函数
  Date() : m_julianDayNumber(0) {}
  Date(SDWORD year, SDWORD month, SDWORD day);
  explicit Date(SDWORD julianDayNum) : m_julianDayNumber(julianDayNum) {}
  explicit Date(const struct tm&);
  
  void swap(Date& that)
  {
    std::swap(m_julianDayNumber, that.m_julianDayNumber);
  }
  bool valid() const { return m_julianDayNumber > 0; }
  std::string toIsoString() const;                      // m_julianDayNumber转换成Iso时间格式，如2020-10-18
  struct YearMonthDay yearMonthDay() const;             // m_julianDayNumber转换成YearMonthDay
  SDWORD year() const { return yearMonthDay().year; }
  SDWORD month() const { return yearMonthDay().month; }
  SDWORD day() const { return yearMonthDay().day; }
  // [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
  SDWORD getWeekDay() const { return (m_julianDayNumber + 1) % kDaysPerWeek; }
  SDWORD julianDayNumber() const { return m_julianDayNumber; }

private:
  SDWORD m_julianDayNumber;                               // 儒略日
};

inline bool operator<(Date x, Date y)
{
  return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(Date x, Date y)
{
  return x.julianDayNumber() == y.julianDayNumber();
}

} // namespace muduo

