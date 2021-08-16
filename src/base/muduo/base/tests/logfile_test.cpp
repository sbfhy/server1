#include "muduo/base/common/logfile.h"
#include "muduo/base/common/logging.h"
#include "muduo/base/define/define_new.h"
#include <unistd.h>
#include <iostream>

using namespace muduo;

std::unique_ptr<muduo::LogFile> g_logFile;

void outputFunc(const CHAR* msg, SDWORD len)
{
  g_logFile->append(msg, len);
}

void flushFunc()
{
  g_logFile->flush();
}

int main(int argc, char* argv[])
{
  CHAR name[256] = { '\0' };
  strncpy(name, argv[0], sizeof name - 1);
  LogFile::setLogFileDir(argv[0]);
  g_logFile.reset(NEW muduo::LogFile(::basename(name), 200*1000));
  muduo::Logger::setOutputFunc(outputFunc);
  muduo::Logger::setFlushFunc(flushFunc);

  std::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  for (SDWORD i = 0; i < 10000; ++i)
  {
    LOG_INFO << line << i;
    usleep(1000);
  }
}
