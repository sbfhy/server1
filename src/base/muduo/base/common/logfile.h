#pragma once

#include "muduo/base/define/define_variable.h"
#include "muduo/base/common/mutex.h"

#include <string>
#include <memory>

namespace muduo
{
namespace FileUtil
{
class AppendFile;
}

/*
 * 封装了日志文件的刷新、滚动。
 * 包含一个AppendFile写入本地文件
 */
class LogFile : private noncopyable
{
public:
  LogFile(const std::string& basename, off_t rollSize,
          bool threadSafe = true, SDWORD flushInterval = 3, SDWORD checkEveryN = 1024);
  ~LogFile();

  void append(const CHAR* logline, SDWORD len);         // 判定m_mutex, 调用append_unlocked   
  void flush();                                         // 判定m_mutex，调用AppendFile->flush()
  bool RollFile();                                      // 日志文件滚动，即新开一个日志文件，m_file重置

  static void SetLogFile(const char* exePath, const char* baseName, off_t rollSize);
  static void setLogFileDir(const char* fileDir);

private:
  void append_unlocked(const CHAR* logline, SDWORD len);  // 检查RollFile()，调用AppendFile->append(2)

  // 根据basename和当前时间，获取日志文件名: basename.20201015-114421.izuf6hyj00tfq6rfwvlyxuz.10294.log
  static std::string getLogFileName(const std::string& basename, time_t* now);

private:
  static std::string s_fileDir;                         // 保存日志文件目录

  const std::string m_basename;                         // 文件名
  const off_t m_rollSize;                               // 文件字节数达到多少时滚动
  const SDWORD m_flushInterval;                         // 刷新间隔 
  const SDWORD m_checkEveryN;                           // 
  SDWORD m_count;                                       // 
  std::unique_ptr<MutexLock> m_mutex;                   // FIXME
  time_t m_startOfPeriod;                               // 
  time_t m_lastRoll;                                    // 上一次文件滚动的时间
  time_t m_lastFlush;                                   // 上一次刷新的时间
  std::unique_ptr<FileUtil::AppendFile> m_file;         // 调用AppendFile 
  const static SDWORD kRollPerSeconds = 60 * 60 * 24;   // 日志文件滚动时间
};

} // namespace muduo

