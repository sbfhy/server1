// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/base/common/async_logging.h"
#include "muduo/base/common/logfile.h"
#include "muduo/base/common/time_stamp.h"
#include "muduo/base/define/define_new.h"

#include <stdio.h>

using namespace muduo;

AsyncLogging::AsyncLogging(const std::string& basename, off_t rollSize, SDWORD flushInterval)
  : m_flushInterval(flushInterval)
  , m_running(false)
  , m_basename(basename)
  , m_rollSize(rollSize)
  , m_thread(std::bind(&AsyncLogging::threadFunc, this), "Logging") // 直接初始化日志线程
  , m_latch(1)                                          // 日志线程中的函数运行起来以后再返回
  , m_mutex()
  , m_cond(m_mutex)
  , m_curBuffer(NEW Buffer)
  , m_nextBuffer(NEW Buffer)
  , m_bufferVec()
{
  m_curBuffer->bzero();
  m_nextBuffer->bzero();
  m_bufferVec.reserve(16);
}

void AsyncLogging::append(const CHAR* logline, SDWORD len)
{
  muduo::MutexLockGuard lock(m_mutex);
  if (m_curBuffer->avail() > len)                       // 如果m_curBuffer够用
  {
    m_curBuffer->append(logline, len);
  }
  else    // 如果满了，就把m_curBuffer里的数据存入m_bufferVec，并将m_nextBuffer转到m_curBuffer
  {
    m_bufferVec.push_back(std::move(m_curBuffer));
    if (m_nextBuffer)
    {
      m_curBuffer = std::move(m_nextBuffer);
    }
    else
    {
      m_curBuffer.reset(NEW Buffer);                    // Rarely happens，临时分配，正常情况应该走threadFunc()里的交换缓冲区
    }
    m_curBuffer->append(logline, len);
    m_cond.notify();                                    // 条件变量通知
  }
}

void AsyncLogging::threadFunc()
{
  assert(m_running == true);
  m_latch.countDown();                                  // 当日志线程运行起来后，Thread的start函数才能继续往下执行
  LogFile output(m_basename, m_rollSize, false);        // LogFile
  BufferPtr newBuffer1(NEW Buffer);                     // 新建两个交换缓冲区
  BufferPtr newBuffer2(NEW Buffer);
  newBuffer1->bzero();
  newBuffer2->bzero();
  BufferVector buffersToWrite;                          // BufferVector，收集要写入的缓冲
  buffersToWrite.reserve(16);
  while (m_running)                                     // 日志线程执行期间循环写入
  {
    assert(newBuffer1 && newBuffer1->length() == 0);
    assert(newBuffer2 && newBuffer2->length() == 0);
    assert(buffersToWrite.empty());

    {
      muduo::MutexLockGuard lock(m_mutex);
      if (m_bufferVec.empty())  // unusual usage!       // 空的，即当前还没有要写入的日志缓存
      {
        m_cond.waitForSeconds(m_flushInterval);         // 等一段时间。一定时间间隔后，就算没有缓存也刷新一下
      }
      m_bufferVec.push_back(std::move(m_curBuffer));
      m_curBuffer = std::move(newBuffer1);
      buffersToWrite.swap(m_bufferVec);                 // m_bufferVec交换
      if (!m_nextBuffer)
      {
        m_nextBuffer = std::move(newBuffer2);
      }
    }

    assert(!buffersToWrite.empty());

    if (buffersToWrite.size() > 25) // 要写的太多，只保留前两个(短时间产生大量日志时，往往只有前面的日志有帮助，所以删掉后面的？)
    {
      CHAR buf[256];
      snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
               TimeStamp::now().toFormattedString().c_str(),
               buffersToWrite.size()-2);
      fputs(buf, stderr);
      output.append(buf, static_cast<SDWORD>(strlen(buf)));
      buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
    }

    for (const auto& buffer : buffersToWrite)
    {
      // FIXME: use unbuffered stdio FILE ? or use ::writev ?
      output.append(buffer->data(), buffer->length());  // 传入到LogFile
    }

    if (buffersToWrite.size() > 2) 
    {
      // drop non-bzero-ed buffers, avoid trashing
      buffersToWrite.resize(2); // 想反复使用前面创建的两个newBuffer1和newBuffer2，尽量把第49行临时分配的Buffer尽早释放。
    }

    if (!newBuffer1)
    {
      assert(!buffersToWrite.empty());
      newBuffer1 = std::move(buffersToWrite.back());    // 复用缓冲区里的旧对象，省去创建的时间
      buffersToWrite.pop_back();
      newBuffer1->reset();
    }
    if (!newBuffer2)
    {
      assert(!buffersToWrite.empty());
      newBuffer2 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer2->reset();
    }

    buffersToWrite.clear();
    output.flush();                                     // LogFile写入到本地文件
  }
  output.flush();
}

