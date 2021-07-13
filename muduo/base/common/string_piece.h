#pragma once

#include "muduo/base/define/define_variable.h"

#include <string>
#include <string_view>

namespace muduo
{

// For passing C-style string argument to a function. 
class StringArg
{
public:
  StringArg(const CHAR* str)
    : m_str(str) {}

  StringArg(const std::string& str)
    : m_str(str.c_str()) {}

  const CHAR* c_str() const { return m_str; }

private:
  const CHAR* m_str;
};

typedef std::string_view StringPiece;

} // namespace muduo

