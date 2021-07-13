#pragma once

namespace muduo
{

/*
 * A tag class emphasises the objects are copyable.
 * The empty base class optimization applies.
 * Any derived class of copyable should be a value type.
 * 用来强调可拷贝，且派生类应是值类型，不能new
 */
class copyable
{
protected:
  copyable() = default;
  ~copyable() = default;
};

} // namespace muduo


