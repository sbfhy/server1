#pragma once

#include "muduo/base/define/define_variable.h"
#include "muduo/base/common/mutex.h"
#include "noncopyable.h"
#include "muduo/base/common/thread.h"
#include "muduo/base/common/count_down_latch.h"
#include "muduo/base/common/log_stream.h"

#include <atomic>
#include <string>
#include <vector>
#include <memory>

namespace muduo
{

/*
 * 日志后端线程，异步日志类，核心。
 * 采用4个以上缓冲区(双缓冲技术)，定时或被填满时将缓冲区数据写入LogFile。
 */
class AsyncLogging : private noncopyable
{
public:
  AsyncLogging(const std::string& basename, off_t rollSize, SDWORD flushInterval = 3);
  virtual ~AsyncLogging()
  {
    if (m_running)
    {
      stop();
    }
  }

  void append(const CHAR* logline, SDWORD len);           // 将logline写入m_curBuffer
  void start()
  {
    m_running = true;
    m_thread.start();
    m_latch.wait();
  }
  void stop() NO_THREAD_SAFETY_ANALYSIS
  {
    m_running = false;
    m_cond.notify();
    m_thread.join();
  }

private:
  /*
   * 双缓冲思想。
   * 两个一直存在的缓冲区m_curBuffer和m_nextBuffer, 两个一直在threadFunc里不停交换的缓冲区。
   * 两个缓冲区容器不停交换，这样可以缩短互斥量锁住的时间，交换完了就可以解锁互斥量。
   */
  void threadFunc();                                    // 该日志线程绑定的函数。循环双缓冲，将容器中存的日志数据写入到本地文件

  typedef muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer> Buffer;
  typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
  typedef BufferVector::value_type BufferPtr;

private:
  const SDWORD m_flushInterval;                         // 容器刷新间隔
  std::atomic<bool> m_running;                          // 是否在运行。主要用于条件变量循环的判断条件
  const std::string m_basename;                         // 文件名
  const off_t m_rollSize;                               // 文件字节数达到多少时滚动
  muduo::Thread m_thread;                               // 该日志线程
  muduo::CountDownLatch m_latch;                        // 控制线程开始运行以后，才继续下面的程序(大概是怕日志线程没准备好就添加日志？)
  muduo::MutexLock m_mutex;                             // 互斥锁
  muduo::Condition m_cond GUARDED_BY(m_mutex);          // 条件变量。当有日志append(2)进来的时候通知，当日志缓冲区没准备好的时候等待
  BufferPtr m_curBuffer GUARDED_BY(m_mutex);            // 正在使用的缓冲区
  BufferPtr m_nextBuffer GUARDED_BY(m_mutex);           // 下一块缓冲区
  BufferVector m_bufferVec GUARDED_BY(m_mutex);         // 容器，保存要写入文件的数据，m_curBuffer满了就往m_bufferVec放
};

} // namespace muduo

