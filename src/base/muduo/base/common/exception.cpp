#include "muduo/base/common/exception.h"
#include "muduo/base/common/current_thread.h"

namespace muduo
{

Exception::Exception(std::string msg)
  : m_message(std::move(msg))
  , m_stack(CurrentThread::stackTrace(/*demangle=*/false))
{

}

} // namespace muduo

