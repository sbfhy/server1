#pragma once

#include "define/define_variable.h"
#include "noncopyable.h"
#include "muduo/base/common/atomic.h"
#include "muduo/base/common/count_down_latch.h"

#include <functional>
#include <string>
#include <pthread.h>

namespace muduo
{

class Thread : private noncopyable
{
public:
  typedef std::function<void ()> ThreadFunc;

  explicit Thread(ThreadFunc, const std::string& name = std::string());
  ~Thread();  // FIXME : make it movable in C++11       // pthread_detach(1)

  void start();                                         // pthread_create(4)
  SDWORD join();                                          // return pthread_join(2)
  
  bool started() const { return m_started; }
  pid_t tid() const { return m_tid; }
  const std::string& name() const { return m_name; }

  static SDWORD numCreated() { return m_numCreated.get(); }

private:
  void setDefaultName();                                // 设置线程默认的名字

  bool            m_started;                            // 线程是否启动，在pthread_create(4)成功后即为true
  bool            m_joined;                             // 在pthread_join(2)之后变为true，即表示是joinable状态还是unjoinable
  pthread_t       m_pthreadId;                          // 子线程id
  pid_t           m_tid;                                // 当前线程id，在ThreadData::runInThread()里更新m_tid
  ThreadFunc      m_func;                               // 线程运行函数，在在ThreadData::runInThread()里调用
  std::string     m_name;                               // 线程名字
  CountDownLatch  m_latch;                              // 倒数器

  static AtomicInt32 m_numCreated;                      // 静态变量，每创建一个线程就+1
};

} // namespace muduo

