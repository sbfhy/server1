#pragma  once

#include "muduo/base/define/define_variable.h"
#include "muduo/base/common/log_stream.h"
#include "muduo/base/common/time_stamp.h"
#include <string.h>

namespace muduo
{

class TimeZone;

class Logger                                            // 日志实体类，使用时定义为匿名对象，保证立即析构 
{
public:
  enum LogLevel
  {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  // 用于保存文件名的类。每行日志消息在编译时就会解析源文件的basename，避免了运行期strrchr(3)开销
  class SourceFile
  {
  public:
    template<SDWORD N>
    SourceFile(const CHAR (&arr)[N])
      : m_data(arr)
      , m_size(N - 1)
    {
      const CHAR* slash = strrchr(m_data, '/');         // 内置函数, strrch找出字符最后出现的位置
      if (slash)
      {
        m_data = slash + 1;
        m_size -= static_cast<SDWORD>(m_data - arr);
      }
    }

    explicit SourceFile(const CHAR* filename)
      : m_data(filename)
    {
      const CHAR* slash = strrchr(filename, '/');
      if (slash)
      {
        m_data = slash + 1;
      }
      m_size = static_cast<SDWORD>(strlen(m_data));
    }

    const CHAR* m_data;                                 // 源文件basename
    SDWORD m_size;
  };

  Logger(SourceFile file, SDWORD line);
  Logger(SourceFile file, SDWORD line, LogLevel level);
  Logger(SourceFile file, SDWORD line, LogLevel level, const CHAR* func);
  Logger(SourceFile file, SDWORD line, bool toAbort);
  virtual ~Logger();

  LogStream& stream() { return m_impl.m_stream; }

  typedef void (*OutputFunc)(const CHAR* msg, SDWORD len);// 函数指针，日志输出函数
  typedef void (*FlushFunc)();                          // 函数指针，日志刷新函数
  static void setOutputFunc(OutputFunc);                // 设置全局的日志输出函数
  static void setFlushFunc(FlushFunc);                  // 设置全局的日志刷新函数
  static void setTimeZone(const TimeZone& tz);          // 设置时区 
  static LogLevel getLogLevel();                        // 日志等级
  static void setLogLevel(LogLevel level);

private:

  // 内部嵌套类，包含一个LogStream。
  // 负责加上一些固定的格式化信息，比如打log的行、文件名等信息
  class Impl
  {
  public:
    typedef Logger::LogLevel LogLevel;
    Impl(LogLevel level, SDWORD oldErrno, const SourceFile& file, SDWORD line);
    void FormatTime();                                  // 将m_time转换成20201018 06:01:37.108001Z格式，存入m_stream中
    void finish();                                      // 最后将m_basename和m_line, 按" - Logging_test.cc:51"格式存入m_stream
  public:
    TimeStamp m_time;
    LogStream m_stream;
    LogLevel m_level;
    SDWORD m_line;
    SourceFile m_basename;
  };

  Impl m_impl;
};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::getLogLevel()
{
  return g_logLevel;
}


/*
// 不能写成：
if (good)
  LOG_INFO << "Good news";
else 
  LOG_WARN << "Bad news";
// 因为展开会是：
if (good)
  if (logging_INFO)
    logInfoStream << "Good news";
  else 
    logWarnStream << "Bad news";
 */
#define LOG_TRACE if (muduo::Logger::getLogLevel() <= muduo::Logger::TRACE) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (muduo::Logger::getLogLevel() <= muduo::Logger::DEBUG) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::DEBUG, __func__).stream()
#define LOG_INFO  if (muduo::Logger::getLogLevel() <= muduo::Logger::INFO) \
  muduo::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN     muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN).stream()
#define LOG_ERROR    muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR).stream()
#define LOG_FATAL    muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL).stream()
#define LOG_SYSERR   muduo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream()

// 自定义的Log, 使用：{LDBG(env) << something;}  
#define LDBG(env)                               \
    static const char* envValue = getenv(env);  \
    if (envValue != nullptr)                    \
        LOG_DEBUG 



const CHAR* strerror_tl(SDWORD savedErrno);

/*
Taken from glog/logging.h
Check that the input is non NULL.  This very useful in constructor initializer lists.
 */
#define CHECK_NOTNULL(val) \
  ::muduo::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL(). 
template <typename T>
T* CheckNotNull(Logger::SourceFile file, SDWORD line, const CHAR* names, T* ptr)
{
  if (ptr == nullptr)
  {
    Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

} // namespace muduo

