#include "muduo/base/common/log_stream.h"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdio.h>
#include <type_traits>

// TODO: better itoa.
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wtautological-compare"
#else
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

namespace muduo
{
namespace detail
{

const CHAR digits[] = "9876543210123456789";            // 数字
const CHAR* zero = digits + 9;                          // 0
static_assert(sizeof(digits) == 20, "wrong number of digits");

const CHAR digitsHex[] = "0123456789ABCDEF";            // 16进制数字
static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

// Efficient Integer to String Conversions, by Matthew Wilson. 
// 数字转换成字符串，填入到buf，返回长度
template<typename T>
size_t convert(CHAR buf[], T value)
{
  T i = value;
  CHAR* p = buf;
  
  do
  {
    SDWORD lsd = static_cast<SDWORD>(i % 10);
    i /= 10;
    *p++ = zero[lsd];                                   // 从低位到高位填入，最后反转一下
  } while (i != 0);

  if (value < 0)
  {
    *p++ = '-';
  }
  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}

// 把uintptr_t转换成16进制字符串，填入到buff，返回长度
size_t convertHex(CHAR buf[], UINTPTR value)
{
  UINTPTR i = value;
  CHAR* p = buf;
  *p++ = '0';
  *p++ = 'x';

  do 
  {
    SDWORD lsd = static_cast<SDWORD>(i % 16);
    i /= 16;
    *p++ = digitsHex[lsd];
  } while (i != 0);

  *p = '\0';
  std::reverse(buf + 2, p);

  return p - buf;
}

// FixedBuffer模板定义两个特例化
template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

} // namespace detail

} // namespace muduo

using namespace muduo;

template<typename T>
void LogStream::formatInteger(T v)
{
  if (m_buffer.avail() >= kMaxNumericSize)                        // 如果m_buffer可用空间大于数字可能的最大字节(32)
  {
    size_t len = detail::convert(m_buffer.current(), v);
    m_buffer.add(len);
  }
}

void LogStream::staticCheck()
{
  // std::numeric_limits<T>::digits10        // T的字节数能表示多少位完整10进制数
  // 以保证用kMaxNumericSize个字节能表示这些类型
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<DOUBLE>::digits10, 
                "kMaxNumericSize is large enough");
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10, 
                "kMaxNumericSize is large enough");
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<INTPTR>::digits10, 
                "kMaxNumericSize is large enough");
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<SQWORD>::digits10, 
                "kMaxNumericSize is large enough");
}

LogStream& LogStream::operator<<(bool v)
{
  m_buffer.append(v ? "1" : "0", 1);
  return *this;
}

LogStream& LogStream::operator<<(SDWORD v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(DWORD v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(SWORD v)
{
  *this << static_cast<SDWORD>(v);  
  return *this;
}

LogStream& LogStream::operator<<(WORD v)
{
  *this << static_cast<DWORD>(v);  
  return *this;
}

// LogStream& LogStream::operator<<(INTPTR v)
// {
//   formatInteger(v);
//   return *this;
// }

// LogStream& LogStream::operator<<(UINTPTR v)
// {
//   formatInteger(v);
//   return *this;
// }

LogStream& LogStream::operator<<(SQWORD v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(QWORD v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(const void* p)
{
  UINTPTR v = reinterpret_cast<UINTPTR>(p);
  if (m_buffer.avail() >= kMaxNumericSize)
  {
    CHAR* buf = m_buffer.current();
    size_t len = detail::convertHex(buf, v);        // 将这个指针转成16进制，存入m_buffer
    m_buffer.add(len);
  }
  return *this;
}

// FIXME: replace this with Grisu3 by Florian Loitsch. 
LogStream& LogStream::operator<<(DOUBLE v)
{
  if (m_buffer.avail() >= kMaxNumericSize)
  {
    // %g根据数值不同自动选择%f或%e; %f单精度浮点数(默认float); %e浮点数、e-记数法;
    // %m.n : m指域宽，即对应的输出项在输出设备上所占的字符数。n指精度。用于说明输出的实型数的小数位数。为指定n时，隐含的精度为n=6位。
    SDWORD len = snprintf(m_buffer.current(), kMaxNumericSize, "%.12g", v);
    m_buffer.add(len);
  }
  return *this;
}

LogStream& LogStream::operator<<(FLOAT v)
{
  *this << static_cast<DOUBLE>(v);
  return *this;
}

LogStream& LogStream::operator<<(CHAR v)
{
  m_buffer.append(&v, 1);
  return *this;
}

LogStream& LogStream::operator<<(const CHAR* str)
{
  if (str)
    m_buffer.append(str, strlen(str));
  else 
    m_buffer.append("(null)", 6);
  return *this;
}

LogStream& LogStream::operator<<(const BYTE* str)
{
  return operator<<(reinterpret_cast<const CHAR*>(str));
}

LogStream& LogStream::operator<<(const std::string& str)
{
  m_buffer.append(str.c_str(), str.size());
  return *this;
}

LogStream& LogStream::operator<<(const StringPiece& v)
{
  m_buffer.append(v.data(), v.size());
  return *this;
}

LogStream& LogStream::operator<<(const Buffer& v)
{
  *this << v.toStringPiece();
  return *this;
}

template<typename T>
Fmt::Fmt(const CHAR* fmt, T val)
{
  static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic type"); // 算术类型
  m_length = snprintf(m_buf, sizeof m_buf, fmt, val);                             // 按fmt格式将数值val转换成字符串，填入到m_buf
  assert(static_cast<size_t>(m_length) < sizeof m_buf);                           // 长度正确
}

// Fmt::Fmt(2) 模板构造函数显式实例化
template Fmt::Fmt(const char* fmt, CHAR);
template Fmt::Fmt(const char* fmt, SWORD);
template Fmt::Fmt(const char* fmt, WORD);
template Fmt::Fmt(const char* fmt, SDWORD);
template Fmt::Fmt(const char* fmt, DWORD);
// template Fmt::Fmt(const char* fmt, INTPTR);
// template Fmt::Fmt(const char* fmt, UINTPTR);
template Fmt::Fmt(const char* fmt, SQWORD);
template Fmt::Fmt(const char* fmt, QWORD);
template Fmt::Fmt(const char* fmt, FLOAT);
template Fmt::Fmt(const char* fmt, DOUBLE);



