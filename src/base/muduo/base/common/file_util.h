#pragma once

#include "noncopyable.h"
#include "muduo/base/common/string_piece.h"
#include "muduo/base/define/define_variable.h"

namespace muduo
{
namespace FileUtil  // 封装了ReadSmallFile和AppendFile，最底层的文件读入和写入类
{

/* 
 * read small file < 64KB
 * 最底层的读入文件类
 */
class ReadSmallFile : private noncopyable
{
public:
  ReadSmallFile(StringArg filename);                    // 构造函数中会打开文件，并记录描述符到m_fd
  ~ReadSmallFile();                                     // 析构函数中关闭文件描述符

  /*
   * 从文件中最多读入maxSize字节，并把读入内容存到content中，返回错误码
   * fileSize: 记录 文件大小(字节)
   * modifyTime: 记录 文件最后修改的时间
   * createTime: 记录 文件最后状态(属性)改变的时间
   */
  template<typename String>
  SDWORD readToString(SDWORD maxSize, String* content, SQWORD* fileSize, SQWORD* modifyTime, SQWORD* createTime);

  // 把文件中内容全部读入缓冲区(最大kBufferSize)，实际读入的字节数赋值给size，返回错误码
  SDWORD readToBuffer(SDWORD* size);

  const CHAR* buffer() const { return m_buf; }

  static const SDWORD kBufferSize = 64 * 1024;            // 缓冲区大小

private:
  SDWORD m_fd;                                            // 文件描述符
  SDWORD m_err;                                           // 错误码
  CHAR m_buf[kBufferSize];                              // 缓冲区
};

// 模板函数，调用ReadSmallFile::readToString()
template<typename String>
SDWORD readFile(StringArg filename, SDWORD maxSize, String* content, 
              SQWORD* fileSize = nullptr, SQWORD* modifyTime = nullptr, SQWORD* createTime = nullptr)
{
  ReadSmallFile file(filename);
  return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

/* 
 * not thread safe，非线程安全，但只会有一个日志线程写入文件
 * 最底层的写入文件类，封装了Log文件的打开、写入并在析构时关闭文件。
 * 底层使用了标准IO，append(2)直接向文件里写
 */
class AppendFile : private noncopyable
{
public:
  explicit AppendFile(StringArg filename);              // 构造函数里打开文件filename，用m_fp操作; 并设置缓冲区m_buffer绑定到m_fp上
  ~AppendFile();                                        // 析构函数里会关闭m_fp

  void append(const CHAR* logline, size_t len);         // 向文件中写入长度为len的logline
  void flush();                                         // 刷新m_fp
  off_t getWrittenBytes() const { return m_writtenBytes; }

private:
  size_t write(const CHAR* logline, size_t len);        // 调用::fwrite_unlocked(4)直接写入文件，返回写入的长度
private:
  FILE* m_fp;                                           // 文件流
  CHAR  m_buffer[64 * 1024];                            // 缓冲区
  off_t m_writtenBytes;                                 // 已经写入文件的总字节数
};

} // namespace FileUtil
} // namespace muduo


