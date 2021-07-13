#include "muduo/base/common/current_thread.h"

#include <cxxabi.h>                                     // FIXME
#include <execinfo.h>

namespace muduo
{
namespace CurrentThread
{
// 线程全局变量
__thread SDWORD t_cachedTid = 0;                        // 记录线程id
__thread CHAR t_tidString[32];                          // 线程id字符串
__thread SDWORD t_tidStringLength = 6;                  // 线程id字符串长度
__thread const CHAR* t_threadName = "unknown";          // 名字
static_assert(std::is_same<SDWORD, pid_t>::value, "pid_t should be int");

std::string stackTrace(bool demangle)                        // 堆栈
{
  std::string stack;
  const SDWORD max_frames = 200;
  void* frame[max_frames];
  SDWORD nptrs = ::backtrace(frame, max_frames);
  CHAR** strings = ::backtrace_symbols(frame, nptrs);
  if (strings)
  {
    size_t len = 256;
    CHAR* demangled = demangle ? static_cast<CHAR*>(::malloc(len)) : nullptr;
    for (SDWORD i = 1; i < nptrs; ++ i)  // skipping the 0-th, which is this function 
    {
      if (demangle)
      {
        // https://panthema.net/2008/0901-stacktrace-demangled
        // bin/exception_test(_ZN3Bar4testEv+0x79) [0x401909]
        CHAR* left_par = nullptr;
        CHAR* plus = nullptr;
        for (CHAR* p = strings[i]; *p; ++ p)
        {
          if (*p == '(')
            left_par = p;
          else if (*p == '+')
            plus = p;
        }

        if (left_par && plus)
        {
          *plus = '\0';
          SDWORD status = 0;
          CHAR* ret = abi::__cxa_demangle(left_par + 1, demangled, &len, &status);
          *plus = '+';
          if (status == 0)
          {
            demangled = ret;  // ret could be realloc()
            stack.append(strings[i], left_par + 1);
            stack.append(demangled);
            stack.append(plus);
            stack.push_back('\n');
            continue;
          }
        }
      }
      // Fallback to mangled names
      stack.append(strings[i]);
      stack.push_back('\n');
    }
    free(demangled);
    free(strings);
  }
  return stack;
}

} // namespace CurrentThread
} // namespace muduo

