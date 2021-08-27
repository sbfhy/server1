#include "muduo/base/common/time_zone.h"
#include "noncopyable.h"
#include "muduo/base/common/date.h"
#include "muduo/base/define/define_new.h"
#include "muduo/base/define/define_types.h"
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>

using std::string;
using std::logic_error;

namespace muduo
{
namespace detail
{

struct Transition                                       // 过渡
{
  time_t gmttime;                                       // 格林威治时间
  time_t localtime;                                     // 当地时间
  SDWORD localtimeIdx;                                    // 对应TimeZone::TimeZoneData.localtimeVec里的序号

  Transition(time_t t, time_t l, SDWORD localIdx)
    : gmttime(t), localtime(l), localtimeIdx(localIdx) {}
};

struct Comp                                             // 封装了一个用于比较的结构
{
  bool compareGmt;                                      // true则用gmttime比较，false则用localtime比较
  
  Comp(bool gmt)
    : compareGmt(gmt) {}

  bool operator()(const Transition& lhs, const Transition& rhs) const 
  {
    if (compareGmt)
      return lhs.gmttime < rhs.gmttime;
    else 
      return lhs.localtime < rhs.localtime;
  }

  bool equal(const Transition& lhs, const Transition& rhs) const 
  {
    if (compareGmt)
      return lhs.gmttime == rhs.gmttime;
    else 
      return lhs.localtime == rhs.localtime;
  }
};

struct Localtime                                        // 当地时间
{
  time_t gmtOffset;                                     // 相对于格林威治时间的偏移量
  bool isDst;                                           // 夏令时
  SDWORD arrbIdx;                                         // 缩写序号

  Localtime(time_t offset, bool dst, SDWORD arrb)
    : gmtOffset(offset), isDst(dst), arrbIdx(arrb) {}
};

inline void fillHMS(DWORD seconds, struct tm* utc)      // 把seconds转成tm里的小时、分钟、秒
{
  utc->tm_sec = seconds % 60;
  DWORD minutes = seconds / 60;
  utc->tm_min = minutes % 60;
  utc->tm_hour = minutes / 60;
}

} // namespace detail
const SDWORD kSecondsPerDay = 24 * 60 * 60;               // 常量，一天多少秒
} // namespace muduo

using namespace muduo;

struct TimeZone::TimeZoneData                           // 时区
{
  std::vector<detail::Transition> transitionVec;        // 
  std::vector<detail::Localtime>  localtimeVec;         // 
  std::vector<string> nameVec;                          // 
  string abbreviation;                                  // 缩写
};

namespace muduo
{
namespace detail
{

class File : private noncopyable                        // 文件类，包含一个FILE*
{
public:
  File(const CHAR* file)
    : m_fp(::fopen(file, "rb")) {}                      // 读入一个文件
  ~File()
  {
    if (m_fp)
      ::fclose(m_fp);
  }

  bool valid() const { return m_fp; }

  string readBytes(SDWORD n)                              // 读入n个字节
  {
    CHAR buf[n];
    ssize_t nr = ::fread(buf, 1, n, m_fp);
    if (nr != n)
      throw logic_error("no enough data");
    return string(buf, n);
  }

  int32_t readInt32()                                   // 读入一个int32_t
  {
    int32_t x = 0;
    ssize_t nr = ::fread(&x, 1, sizeof(int32_t), m_fp);
    if (nr != sizeof(int32_t))
      throw logic_error("bad int32_t data");
    return be32toh(x);
  }

  uint8_t readUInt8()                                   // 读入一个uint8_t
  {
    uint8_t x = 0;
    ssize_t nr = ::fread(&x, 1, sizeof(uint8_t), m_fp);
    if (nr != sizeof(uint8_t))
      throw logic_error("bad uint8_t data");
    return x;
  }

private:
  FILE* m_fp;                                           // 系统定义的文件操作结构
};

// 读入时区配置文件
bool readTimeZoneFile(const CHAR* zonefile, struct TimeZone::TimeZoneData* data)
{
  File f(zonefile);
  if (f.valid())
  {
    try
    {
      string head = f.readBytes(4);
      if (head != "TZif")
        throw logic_error("bad head");
      string version = f.readBytes(1);
      f.readBytes(15);

      int32_t isgmtcnt = f.readInt32();
      int32_t isstdcnt = f.readInt32();
      int32_t leapcnt = f.readInt32();
      int32_t timecnt = f.readInt32();
      int32_t typecnt = f.readInt32();
      int32_t charcnt = f.readInt32();

      std::vector<int32_t> trans;
      std::vector<SDWORD> localtimes;
      trans.reserve(timecnt);
      for (SDWORD i = 0; i < timecnt; ++ i)
      {
        trans.push_back(f.readInt32());
      }
      for (SDWORD i = 0; i < timecnt; ++ i)
      {
        uint8_t local = f.readUInt8();
        localtimes.push_back(local);
      }
      for (SDWORD i = 0; i < typecnt; ++ i)
      {
        int32_t gmtoff = f.readInt32();
        uint8_t isdst = f.readUInt8();
        uint8_t abbrind = f.readUInt8();
        data->localtimeVec.push_back(Localtime(gmtoff, isdst, abbrind));
      }
      for (SDWORD i = 0; i < timecnt; ++ i)
      {
        SDWORD localIdx = localtimes[i];
        time_t localtime = trans[i] + data->localtimeVec[localIdx].gmtOffset;
        data->transitionVec.push_back(Transition(trans[i], localtime, localIdx));
      }

      data->abbreviation = f.readBytes(charcnt);

      for (SDWORD i = 0; i < leapcnt; ++ i)               // leapcnt
      {
        //int32_t leaptime = f.readInt32();
        //int32_t cumleap = f.readInt32();
      }
      // FIXME
      (void) isstdcnt;
      (void) isgmtcnt;
    }
    catch (logic_error& e)
    {
      fprintf(stderr, "%s\n", e.what());
    }
  }
  return true;
}

// 找出data里最接近sentry的localtime
const Localtime* FindLocaltime(const TimeZone::TimeZoneData& data, Transition sentry, Comp comp)
{
  const Localtime* local = nullptr;
  if (data.transitionVec.empty() || comp(sentry, data.transitionVec.front()))
  {
    // FIXME: should be first non dst time zone 
    local = &data.localtimeVec.front();
  }
  else
  {
    std::vector<Transition>::const_iterator transI = std::lower_bound(data.transitionVec.begin(), 
                                                                      data.transitionVec.end(), 
                                                                      sentry, 
                                                                      comp);
    if (transI != data.transitionVec.end())
    {
      if (!comp.equal(sentry, *transI))
      {
        assert(transI != data.transitionVec.begin());
        -- transI;
      }
      local = &data.localtimeVec[transI->localtimeIdx];
    }
    else 
    {
      // FIXME: use TZ-env 
      local = &data.localtimeVec[data.transitionVec.back().localtimeIdx];
    }
  }
  return local;
}

} // namespace detail
} // namespace muduo


TimeZone::TimeZone(const CHAR* zonefile)
  : m_data(NEW TimeZone::TimeZoneData)
{
  if (!detail::readTimeZoneFile(zonefile, m_data.get()))
  {
    m_data.reset();
  }
}

TimeZone::TimeZone(SDWORD eastOfUtc, const CHAR* name)
  :m_data(NEW TimeZone::TimeZoneData)
{
  m_data->localtimeVec.push_back(detail::Localtime(eastOfUtc, false, 0));
  m_data->abbreviation = name;
}

struct tm TimeZone::toLocalTime(time_t seconds) const 
{
  struct tm localTime;
  memZero(&localTime, sizeof(localTime));
  assert(m_data != nullptr);                            // FIXME
  const TimeZoneData& data(*m_data);
  detail::Transition sentry(seconds, 0, 0);
  const detail::Localtime* local = FindLocaltime(data, sentry, detail::Comp(true));
  if (local)
  {
    time_t localSeconds = seconds + local->gmtOffset;
    ::gmtime_r(&localSeconds, &localTime);              // FIXME: fromUtcTime
    localTime.tm_isdst = local->isDst;
    localTime.tm_gmtoff = local->gmtOffset;
    localTime.tm_zone = &data.abbreviation[local->arrbIdx];
  }
  return localTime;
}

time_t TimeZone::fromLocalTime(const struct tm& localTm) const 
{
  assert(m_data != nullptr);                            // FIXME
  const TimeZoneData& data(*m_data);
  struct tm tmp = localTm;
  time_t seconds = ::timegm(&tmp);                      // FIXME: toUtcTime 
  detail::Transition sentry(0, seconds, 0);
  const detail::Localtime* local = FindLocaltime(data, sentry, detail::Comp(false));
  if (localTm.tm_isdst)
  {
    struct tm tryTm = toLocalTime(seconds - local->gmtOffset);
    if (!tryTm.tm_isdst                                 // 夏令时
        && tryTm.tm_hour == localTm.tm_hour 
        && tryTm.tm_min == localTm.tm_min)
    {
      // FIXME: HACK
      seconds -= 3600;
    }
  }
  return seconds - local->gmtOffset;
}

struct tm TimeZone::toUtcTime(time_t secSinceEpoch, bool yday)
{
  struct tm utc;
  memZero(&utc, sizeof(utc));
  utc.tm_zone = "GMT";
  SDWORD seconds = static_cast<SDWORD>(secSinceEpoch % kSecondsPerDay);
  SDWORD days = static_cast<SDWORD>(secSinceEpoch / kSecondsPerDay);
  if (seconds < 0)
  {
    seconds += kSecondsPerDay;
    -- days;
  }
  detail::fillHMS(seconds, &utc);
  Date date(days + Date::kJulianDayOf19700101);
  Date::YearMonthDay ymd = date.yearMonthDay();
  utc.tm_year = ymd.year - 1900;
  utc.tm_mon = ymd.month - 1;
  utc.tm_mday = ymd.day;
  utc.tm_wday = date.getWeekDay();
  if (yday)
  {
    Date startOfYear(ymd.year, 1, 1);
    utc.tm_yday = date.julianDayNumber() - startOfYear.julianDayNumber();
  }
  return utc;
}

time_t TimeZone::fromUtcTime(const struct tm& utc)
{
  return fromUtcTime(utc.tm_year + 1900, utc.tm_mon + 1, utc.tm_mday,
                     utc.tm_hour, utc.tm_min, utc.tm_sec);
}

time_t TimeZone::fromUtcTime(SDWORD year, SDWORD month, SDWORD day, SDWORD hour, SDWORD minute, SDWORD seconds)
{
  Date date(year, month, day);
  SDWORD secondsInDay = hour * 3600 + minute * 60 + seconds;
  time_t days = date.julianDayNumber() - Date::kJulianDayOf19700101;
  return days * kSecondsPerDay + secondsInDay;
}


