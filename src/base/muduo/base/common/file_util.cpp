#include "muduo/base/common/file_util.h"
#include "muduo/base/common/logging.h"
#include "define/define_types.h"

#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>

using namespace muduo;
using namespace muduo::FileUtil;

AppendFile::AppendFile(StringArg filename)
  : m_fp(::fopen(filename.c_str(), "ae"))               // 'e' for O_CLOEXEC 
  , m_writtenBytes(0)
{
  assert(m_fp);
  ::setbuffer(m_fp, m_buffer, sizeof m_buffer);         // 设置缓冲区
}

AppendFile::~AppendFile()
{
  ::fclose(m_fp);
}

void AppendFile::append(const CHAR* logline, const size_t len)
{
  size_t n = write(logline, len);                       // n为成功写入的长度
  size_t remain = len - n;                              // remain为剩余还需写入的长度
  while (remain > 0)
  {
    size_t x = write(logline + n, remain);
    if (x == 0)
    {
      SDWORD err = ferror(m_fp);
      if (err)
      {
        fprintf(stderr, "AppendFile::Append() failed %s \n", strerror_tl(err));
      }
      break;
    }
    n += x;
    remain = len - n;
  }
  m_writtenBytes += len;
}

void AppendFile::flush()
{
  ::fflush(m_fp);
}

size_t AppendFile::write(const CHAR* logline, size_t len)
{
  // #undef fwrite_unlocked
  return ::fwrite_unlocked(logline, 1, len, m_fp);
}

ReadSmallFile::ReadSmallFile(StringArg filename)
  // 调用open函数，O_CLOEXEC模式打开的文件描述符在执行exec调用新程序中关闭，且为原子操作。
  : m_fd(::open(filename.c_str(), O_RDONLY | O_CLOEXEC))
  , m_err(0)
{
  m_buf[0] = '\0';
  if (m_fd < 0)
  {
    m_err = errno;
  }
}

ReadSmallFile::~ReadSmallFile()
{
  if (m_fd >= 0)
  {
    ::close(m_fd);                                      // FIXME: check EINTR
  }
}

// return errno
template<typename String>
SDWORD ReadSmallFile::readToString(SDWORD maxSize, String* content, SQWORD* fileSize, SQWORD* modifyTime, SQWORD* createTime)
{
  static_assert(sizeof(off_t) == 8, "_FILE_OFFSET_BITS = 64");
  assert(content != nullptr);
  SDWORD err = m_err;
  if (m_fd >= 0)
  {
    content->clear();                                   // 清空
    if (fileSize)
    {
      struct stat statbuf;                              // 获取文件信息到statbuf
      if (::fstat(m_fd, &statbuf) == 0)
      {
        if (S_ISREG(statbuf.st_mode))                   // 是否常规文件
        {
          *fileSize = statbuf.st_size;                  // 文件大小(字节)
          content->reserve(static_cast<SDWORD>(std::min(implicit_cast<SQWORD>(maxSize), *fileSize)));
        }
        else if (S_ISDIR(statbuf.st_mode))              // 是否目录
        {
          err = EISDIR;
        }
        if (modifyTime)
        {
          *modifyTime = statbuf.st_mtime;               // 文件最后修改的时间
        }
        if (createTime)
        {
          *createTime = statbuf.st_ctime;               // 文件最后状态(属性)改变的时间
        }
      }
      else
      {
        err = errno;
      }
    }

    while (content->size() < implicit_cast<size_t>(maxSize))
    {
      // toRead 确定要读入的字节数
      size_t toRead = std::min(implicit_cast<size_t>(maxSize) - content->size(), sizeof(m_buf));
      ssize_t n = ::read(m_fd, m_buf, toRead);  // 直接从文件m_fd中读入，存到m_buf，返回实际读入的字节数
      if (n > 0)
      {
        content->append(m_buf, n);
      }
      else 
      {
        if (n < 0)
        {
          err = errno;
        }
        break;
      }
    }
  }
  return err;
}

SDWORD ReadSmallFile::readToBuffer(SDWORD* size)
{
  SDWORD err = m_err;
  if (m_fd >= 0)
  {
    /*
     * pread()  reads  up to count bytes from file descriptor fd at offset offset (from the start of the 
     * file) into the buffer starting at buf.  The file offset is not changed.
     */
    ssize_t n = ::pread(m_fd, m_buf, sizeof(m_buf) - 1, 0);
    if (n >= 0)
    {
      if (size)
      {
        *size = static_cast<SDWORD>(n);
      }
      m_buf[n] = '\0';
    }
    else 
    {
      err = errno;
    }
  }
  return err;
}

// 模板实例
template SDWORD readFile(StringArg filename, SDWORD maxSize, std::string* content, SQWORD*, SQWORD*, SQWORD*); 
template SDWORD ReadSmallFile::readToString(SDWORD maxSize, std::string* content, SQWORD*, SQWORD*, SQWORD*);

