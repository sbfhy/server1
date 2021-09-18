#pragma once

#include "define/define_variable.h"

#include <string>

namespace muduo
{
namespace CurrentThread
{
  // internal
  extern __thread SDWORD t_cachedTid;
  extern __thread CHAR t_tidString[32];
  extern __thread SDWORD t_tidStringLength;
  extern __thread const CHAR* t_threadName;

  void cacheTid();                                      // 如果t_cachedTid==0，就通过系统调用获取一下t_cachedTid、t_tidString

  inline SDWORD tid()                                   // 获取线程id
  {
    if (__builtin_expect(t_cachedTid == 0, 0))          // 分支预测优化，表示执行if分支的可能性很小。减少指令跳转带来的性能上的下降
    {
      cacheTid();
    }
    return t_cachedTid;
  }

  inline const CHAR* tidString() { return t_tidString; }      // for logging
  inline SDWORD tidStringLength() { return t_tidStringLength; } // for logging
  inline const CHAR* name() { return t_threadName; }

  bool isMainThread();                                  // 是否主线程
  void sleepUsec(SQWORD usec);                          // for testing，
  std::string stackTrace(bool demangle);                // 堆栈

} // namespace CurrentThread
} // namespace muduo


