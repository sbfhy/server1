#include "muduo/base/common/logfile.h"

#include "muduo/base/common/logging.h"
#include "muduo/base/common/file_util.h"
#include "muduo/base/common/process_info.h"
#include "define/define_new.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <libgen.h>

using namespace muduo;

std::string LogFile::s_fileDir = "log/";
std::unique_ptr<muduo::LogFile> g_logFile;

void outputFunc(const CHAR* msg, SDWORD len)
{
  g_logFile->append(msg, len);
}

void flushFunc()
{
  g_logFile->flush();
}


LogFile::LogFile(const std::string& basename, off_t rollSize,
                 bool threadSafe, SDWORD flushInterval, SDWORD checkEveryN)
  : m_basename(basename)
  , m_rollSize(rollSize)
  , m_flushInterval(flushInterval)
  , m_checkEveryN(checkEveryN)
  , m_count(0)
  , m_mutex(threadSafe ? NEW MutexLock : nullptr)
  , m_startOfPeriod(0)
  , m_lastRoll(0) 
  , m_lastFlush(0)
{
  assert(basename.find('/') == std::string::npos);
  RollFile();
}

LogFile::~LogFile() = default;

void LogFile::append(const CHAR* logline, SDWORD len)
{
  if (m_mutex)
  {
    MutexLockGuard lock(*m_mutex);
    append_unlocked(logline, len);
  }
  else
  {
    append_unlocked(logline, len);
  }
}

void LogFile::flush()
{
  if (m_mutex)
  {
    MutexLockGuard lock(*m_mutex);
    m_file->flush();
  }
  else
  {
    m_file->flush();
  }
}

void LogFile::append_unlocked(const CHAR* logline, SDWORD len)
{
  m_file->append(logline, len);
  if (m_file->getWrittenBytes() > m_rollSize)
  {
    RollFile();
  }
  else 
  {
    ++ m_count;
    if (m_count >= m_checkEveryN)
    {
      m_count = 0;
      time_t now = ::time(nullptr);
      time_t thisPeriod = now / kRollPerSeconds * kRollPerSeconds;
      if (thisPeriod != m_startOfPeriod)
      {
        RollFile();
      }
      else if (now - m_lastFlush > m_flushInterval)
      {
        m_lastFlush = now;
        m_file->flush();
      }
    }
  }
}

bool LogFile::RollFile()
{
  time_t now = 0;
  std::string filename = getLogFileName(m_basename, &now);
  time_t start = now / kRollPerSeconds * kRollPerSeconds;
  if (now > m_lastRoll)
  {
    m_lastRoll = now;
    m_lastFlush = now;
    m_startOfPeriod = start;
    m_file.reset(NEW FileUtil::AppendFile(LogFile::s_fileDir + filename));
    return true;
  }
  return false;
}

void LogFile::SetLogFile(const char* exePath, const char* baseName, off_t rollSize)
{
    g_logFile.reset(NEW muduo::LogFile(baseName, rollSize));
    muduo::Logger::setOutputFunc(outputFunc);
    muduo::Logger::setFlushFunc(flushFunc);
}

void LogFile::setLogFileDir(const char* fileDir)
{
  s_fileDir = ::dirname(const_cast<char*>(fileDir));
  s_fileDir += "/log/";
}

std::string LogFile::getLogFileName(const std::string& basename, time_t* now)
{
  std::string filename;
  filename.reserve(basename.size() + 64);
  filename = basename;
  CHAR timebuf[32];
  struct tm tm;
  *now = time(nullptr);
  //gmtime_r(now, &tm);                                   // FIXME: localtime_r ?
  localtime_r(now, &tm);
  strftime(timebuf, sizeof timebuf, "_%Y%m%d_%H%M%S_", &tm);
  filename += timebuf;
//   filename += ProcessInfo::hostname();
  
  CHAR pidbuf[32];
  snprintf(pidbuf, sizeof pidbuf, "%d", ProcessInfo::pid());
  filename += pidbuf;
  filename += ".log";
  return filename;
}

