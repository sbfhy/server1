#pragma once

namespace muduo
{

class noncopyable
{
public:
  noncopyable(const noncopyable&) = delete;
  void operator = (const noncopyable&) = delete;

protected:
  noncopyable() = default;
  virtual ~noncopyable() = default;
};

} // namespace muduo
