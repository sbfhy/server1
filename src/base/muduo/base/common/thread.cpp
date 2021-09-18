#include "muduo/base/common/thread.h"
#include "muduo/base/common/time_stamp.h"
#include "muduo/base/common/logging.h"
#include "muduo/base/common/exception.h"
#include "define/define_new.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <unistd.h>

namespace muduo
{
namespace detail
{

pid_t gettid()                                          // 通过系统调用获取线程id唯一标识tid
{
  return static_cast<pid_t>(::syscall(SYS_gettid));     // 系统调用syscall(SYS_gettid)
}

void afterFork()                                        // 在fork()之后，子进程信息初始化
{
  CurrentThread::t_cachedTid = 0;
  CurrentThread::t_threadName = "main";
  CurrentThread::tid();
  // no need to call pthread_atfork(NULL, NULL, &afterFork); 
}

class ThreadNameInitializer                             // 封装线程名字初始化的操作
{
public:
  ThreadNameInitializer()
  {
    CurrentThread::t_threadName = "main";
    CurrentThread::tid();
    /*
     * pthread_atfork(prepare, parent, child);
     * pthread_atfork()在fork()之前调用，当调用fork时，内部创建子进程前在父进程中会调用prepare，内部创建子进程成功后，父进程会调用parent ，子进程会调用child。
     * 可以用pthread_atfork()来解决子进程死锁问题, 在prepare()中将子线程会用到的mutex解锁，然后在parent()中再次加锁。
     */
    pthread_atfork(nullptr, nullptr, &afterFork);
  }
};

ThreadNameInitializer init;                             // 构造时初始化当前线程

struct ThreadData                                       // 线程记录数据的结构
{
  typedef muduo::Thread::ThreadFunc ThreadFunc;
  ThreadFunc m_func;                                    // 线程函数
  std::string m_name;                                   // 名字
  pid_t* m_tid;                                         // 线程id
  CountDownLatch* m_latch;                              // 倒数器

  ThreadData(ThreadFunc func, const std::string& name, pid_t* tid, CountDownLatch* latch)
    : m_func(std::move(func))
    , m_name(name)
    , m_tid(tid)
    , m_latch(latch) { }

  void runInThread()                                    // 运行函数，主要是执行m_func
  {
    *m_tid = muduo::CurrentThread::tid();
    m_tid = nullptr;
    m_latch->countDown();
    m_latch = nullptr;

    muduo::CurrentThread::t_threadName = m_name.empty() ? "muduoThread" : m_name.c_str();
    ::prctl(PR_SET_NAME, muduo::CurrentThread::t_threadName);
    try
    {
      m_func();
      muduo::CurrentThread::t_threadName = "finished";
    }
    catch (const Exception& ex)
    {
      muduo::CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", m_name.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
      abort();
    }
    catch (const std::exception& ex)
    {
      muduo::CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", m_name.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      abort();
    }
    catch (...)
    {
      muduo::CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "unknown exception caught in Thread %s\n", m_name.c_str());
      throw;  // rethrow
    }
  }
};

void* startThread(void* obj)                            // 开始执行ThreadData::runInThread()
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  DELETE(data);
  return nullptr;
}

} // namespace detail

void CurrentThread::cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = detail::gettid();                     // 通过系统调用获取线程id
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

bool CurrentThread::isMainThread()
{
  return tid() == ::getpid();
}

void CurrentThread::sleepUsec(SQWORD usec)
{
  struct timespec ts = { 0, 0 };
  ts.tv_sec = static_cast<time_t>(usec / TimeStamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<INTPTR>(usec % TimeStamp::kMicroSecondsPerSecond * 1000);
  ::nanosleep(&ts, nullptr);                            // 睡眠ts时间
}


AtomicInt32 Thread::m_numCreated;

Thread::Thread(ThreadFunc func, const std::string& n)
  : m_started(false)
  , m_joined(false)
  , m_pthreadId(0)
  , m_tid(0)
  , m_func(std::move(func))
  , m_name(n)
  , m_latch(1)
{
  setDefaultName();
}

Thread::~Thread()
{
  if (m_started && !m_joined) // 如果线程还开着，但不是joinable状态，这说明线程没有自动回收资源，还保留在退出状态，等待其它线程join
  {
    pthread_detach(m_pthreadId);  // 将m_pthreadId线程分离，该线程将自动回收资源
  }
}

void Thread::setDefaultName()
{
  SDWORD num = m_numCreated.incrementAndGet();
  if (m_name.empty())
  {
    CHAR buf[32];
    snprintf(buf, sizeof buf, "Thread%d", num);
    m_name = buf;
  }
}

void Thread::start()
{
  assert(!m_started);
  m_started = true;
  // FIXME: move(func_)
  detail::ThreadData* data = NEW detail::ThreadData(m_func, m_name, &m_tid, &m_latch);
  /*
   * int  pthread_create(pthread_t *tidp, const pthread_attr_t *attr, ( void * )(*start_rtn)( void * ), void  *arg);
   * 成功返回0，否则返回错编号。
   * 返回成功时，由tidp指向的内存单元被设置为新创建线程的线程ID。
   * attr参数用于指定各种不同的线程属性。
   * 新创建的线程从start_rtn函数的地址开始运行，是线程运行函数的起始地址。
   * arg是要传入的指针参数，运行函数的参数。
   */
  // 即创建一个线程，m_pthreadId保存新线程的线程id，然后新线程运行detail::startThread(data)
  if (pthread_create(&m_pthreadId, nullptr, &detail::startThread, data))
  {
    m_started = false;
    DELETE(data); // or no delete? 
    LOG_SYSFATAL << "Failed in pthread_create";
  }
  else 
  {
    m_latch.wait();
    assert(m_tid > 0);
  }
}

SDWORD Thread::join()
{
  assert(m_started);
  assert(!m_joined);
  m_joined = true;
  /*
   * int pthread_join(pthread_t thread, void **retval);
   * 以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。
   * 如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的。
   */
  return pthread_join(m_pthreadId, nullptr);
}

} // namespace muduo

