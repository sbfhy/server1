#pragma once

#include "define/define_variable.h"
#include <exception>
#include <string>

namespace muduo
{

// 封装异常, try, catch, throw
class Exception : public std::exception
{
public:
  Exception(std::string what);
  ~Exception() noexcept override = default;

  const CHAR* what() const noexcept override
  {
    return m_message.c_str();
  }

  const CHAR* stackTrace() const noexcept
  {
    return m_stack.c_str();
  }

private:
  std::string m_message;
  std::string m_stack;
};

} // namespace muduo

