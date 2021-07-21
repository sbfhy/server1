#include "muduo/base/common/logging.h"
#include "muduo/base/common/time_zone.h"
#include "muduo/base/common/current_thread.h"
#include <cassert>
#include <cerrno>
#include <cstring>
#include <errno.h>

namespace muduo
{

// 线程局部变量
__thread CHAR t_errnobuf[512];                          // 错误信息记录
__thread CHAR t_time[64];                               // Logger::Impl::FormatTime()里用来记录时间的字符串
__thread time_t t_lastSecond;                           // 上一次调用Logger::Impl::FormatTime()的秒数

const CHAR* strerror_tl(SDWORD savedErrno)                // 获取某个错误码对应的错误信息
{
  /*
   * #include <string.h>, 返回字符串，用来描述errnum
   * 函数原型：int strerror_r(int errnum, char *buf, size_t buflen);
   * errnum 错误号，buf 储存错误信息，buflen 即buf的长度
   */
  return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

Logger::LogLevel initLogLevel()                         // 默认从环境变量初始化日志等级
{
  // FIXME 暂时修改
  return Logger::DEBUG;
  //   return Logger::TRACE;
  // if (::getenv("MUDUO_LOG_TRACE"))
  //   return Logger::TRACE;
  // else if (::getenv("NUDUO_LOG_DEBUG"))
  //   return Logger::DEBUG;
  // else 
  //   return Logger::INFO;
}

const CHAR* LogLevelName[Logger::NUM_LOG_LEVELS] =      // 要保证字段长度一致
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

// helper class for known string length at compile time
class T             // 工具类，保存一个字符串和长度。用于在编译期获取长度，运行时不用调用strlen
{
public:
  T(const CHAR* str, DWORD len)
    : m_str(str)
      , m_len(len)
  {
    assert(strlen(str) == m_len);
  }

  const CHAR* m_str;
  const DWORD m_len;
};

inline LogStream& operator<<(LogStream& s, T v)
{
  s.append(v.m_str, v.m_len);
  return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
  s.append(v.m_data, v.m_size);
  return s;
}

void defaultOutput(const char* msg, int len)
{
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  (void)n;
}

void defaultFlush()
{
  fflush(stdout);
}

// 全局变量
Logger::LogLevel g_logLevel = initLogLevel();
Logger::OutputFunc g_outputFunc = defaultOutput;
Logger::FlushFunc  g_flushFunc = defaultFlush;
TimeZone g_logTimeZone;                                 // 时区

} // namespace muduo

using namespace muduo;

Logger::Impl::Impl(LogLevel level, SDWORD savedErrno, const SourceFile& file, SDWORD line)
  : m_time(TimeStamp::now())
  , m_stream()
  , m_level(level)
  , m_line(line)
  , m_basename(file)
{
  FormatTime();
  CurrentThread::tid();                                 // FIXME
  m_stream << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
  m_stream << T(LogLevelName[level], 6);
  if (savedErrno != 0)
  {
    m_stream << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

void Logger::Impl::FormatTime()
{
  QWORD curUSec = m_time.getUSec();
  time_t sec = static_cast<time_t>(curUSec / TimeStamp::kMicroSecondsPerSecond);
  SDWORD uSec = static_cast<SDWORD>(curUSec % TimeStamp::kMicroSecondsPerSecond);
  if (sec != t_lastSecond)
  {
    t_lastSecond = sec;
    struct tm tm_time;
    if (g_logTimeZone.valid())
    {
      tm_time = g_logTimeZone.toLocalTime(sec);
    }
    else
    {
      ::gmtime_r(&sec, &tm_time);   // FIXME TimeZone::fromUtcTime
    }

    SDWORD len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
                        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday, 
                        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17);  (void)len;
  }
  if (g_logTimeZone.valid())
  {
    Fmt us(".%06d ", uSec);
    assert(us.length() == 8);
    m_stream << T(t_time, 17) << T(us.data(), 8);
  }
  else 
  {
    Fmt us(".%06dZ ", uSec);                            // 没定义时区，多输出一个Z
    assert(us.length() == 9);
    m_stream << T(t_time, 17) << T(us.data(), 9);
  }
}

void Logger::Impl::finish()
{
  m_stream << " - " << m_basename << ':' << m_line << '\n';
}

Logger::Logger(SourceFile file, SDWORD line)
  : m_impl(INFO, 0, file, line)
{
}

Logger::Logger(SourceFile file, SDWORD line, LogLevel level, const CHAR* func)
  : m_impl(level, 0, file, line)
{
  m_impl.m_stream << func << ' ';
}

Logger::Logger(SourceFile file, SDWORD line, LogLevel level)
  : m_impl(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, SDWORD line, bool toAbort)
  : m_impl(toAbort?FATAL:ERROR, errno, file, line)
{
}

Logger::~Logger()
{
  m_impl.finish();
  const LogStream::Buffer& buf(stream().buffer());
  g_outputFunc(buf.data(), buf.length());
  if (m_impl.m_level == FATAL)                          // fatal级别，终止程序
  {
    g_flushFunc();
    abort();                                            // FIXME
  }
}

void Logger::setLogLevel(Logger::LogLevel level)
{
  g_logLevel = level;
}

void Logger::setOutputFunc(OutputFunc outputFunc)
{
  g_outputFunc = outputFunc;
}

void Logger::setFlushFunc(FlushFunc flushFunc)
{
  g_flushFunc = flushFunc;
}

void Logger::setTimeZone(const TimeZone& tz)
{
  g_logTimeZone = tz;
}


