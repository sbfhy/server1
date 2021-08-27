#pragma once

#include "muduo/base/define/define_variable.h"
#include "noncopyable.h"
#include "muduo/base/define/define_types.h"
#include "muduo/base/common/string_piece.h"

#include <string>
#include <string.h>                                               // memcpy

namespace muduo
{
namespace detail
{

const SDWORD kSmallBuffer = 4000;                                   // 
const SDWORD kLargeBuffer = 4000 * 1000;                            // 

// 缓冲区模板类，传入非类型参数SIZE表示缓冲区的大小
template<SDWORD SIZE>
class FixedBuffer : private noncopyable
{
public:
  FixedBuffer()
    : m_cur(m_data)
  {
  }

  ~FixedBuffer()
  {
  }

  void append(const CHAR* /*restrict*/ buf, size_t len)           // 在m_data尾端拼接上buf
  {
    if (implicit_cast<size_t>(avail()) > len)
    {
      memcpy(m_cur, buf, len);
      m_cur += len;
    }
  }
  const CHAR* data() const { return m_data; }
  int length() const { return static_cast<SDWORD>(m_cur - m_data); }
  CHAR* current() { return m_cur; }
  SDWORD avail() const { return static_cast<SDWORD>(end() - m_cur); } // 获取m_data里当前可用的长度
  void add(size_t len) { m_cur += len; }
  void reset() { m_cur = m_data; }
  void bzero() { memZero(m_data, sizeof m_data); }
  std::string toString() const { return std::string(m_data, length()); }      // 获取string形式
  StringPiece toStringPiece() const { return StringPiece(m_data, length()); } // 获取StringPiece形式

private:
  const CHAR* end() const { return m_data + sizeof m_data; }

private:
  CHAR m_data[SIZE];                                              // 数据
  CHAR* m_cur;                                                    // 当前位置指针
};

} // namespace detail


// 自定义的日志流结构，里面包含一个FixedBuffer作为缓冲区
class LogStream : private noncopyable
{
  typedef LogStream self;
public:
  typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

  self& operator<<(bool);
  self& operator<<(SDWORD);
  self& operator<<(DWORD);
  self& operator<<(SWORD);
  self& operator<<(WORD);
//   self& operator<<(INTPTR);
//   self& operator<<(UINTPTR);
  self& operator<<(SQWORD);
  self& operator<<(QWORD);
  self& operator<<(const void*);
  self& operator<<(DOUBLE);
  self& operator<<(FLOAT);
  self& operator<<(CHAR);
  self& operator<<(const CHAR*);
  self& operator<<(const BYTE*);
  self& operator<<(const std::string&);
  self& operator<<(const StringPiece&);
  self& operator<<(const Buffer&);

  void append(const CHAR* data, SDWORD len) { m_buffer.append(data, len); }
  const Buffer& buffer() const { return m_buffer; }
  void ResetBuffer() { m_buffer.reset(); }

private:
  template<typename T>
  void formatInteger(T);                                          // 把T 转换成10进制数字字符串，存入m_buffer
  void staticCheck();                                             // 静态检查

private:
  Buffer m_buffer;                                                // 缓冲区，日志消息先输到这个缓冲区，之后再集中输出到标准输出或文件
  static const SDWORD kMaxNumericSize = 32;                         // 一个数字最多可以占用多少字节
};

// 包含一个32字节的字符串，模板构造函数。一个算术数值可以构造一个Fmt对象
class Fmt
{
public:
  template<typename T>                                            // 构造函数模板。按fmt格式将数值val转换成字符串，填入到m_buf
  Fmt(const CHAR* fmt, T val);

  const CHAR* data() const { return m_buf; }
  SDWORD length() const { return m_length; }

private:
  CHAR m_buf[32];
  SDWORD m_length;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
{
  s.append(fmt.data(), fmt.length());
  return s;
}

} // namespace muduo 

