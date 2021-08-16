#pragma once 

#include "muduo/base/common/mutex.h"
#include "muduo/base/common/current_thread.h"
#include "muduo/base/common/time_stamp.h"
#include "muduo/net/common/timerid.h"
#include "muduo/net/common/callbacks.h"
#include <any>
#include <atomic>
#include <functional>
#include <vector>

namespace muduo
{
namespace net
{

class Channel;
class Poller;
class TimerQueue;

/*
 * Reactor, at most one per thread.  // Reactor，注：原则是一个线程最多一个EventLoop。
 * This is an interface class, so don't expose too much details.  // 并不拥有Channel对象
 */
class EventLoop : private noncopyable
{
public:
  typedef std::function<void()> Functor;
  
  EventLoop();
  virtual ~EventLoop(); // force out-line dtor, for std::unique_ptr members.

  /*
   * Loops forever.
   * Must be called in the same thread as creation of the object. // 必须在创建该对象的线程里调用Loop()
   */
  virtual void Loop();

  /*
   * Quits loop.
   * This is not 100% thread safe, if you call through a raw pointer, better to call through shared_ptr<EventLoop> for 100% safety. 
   * 用原始指针调用Quit()是非线程安全的
   */
  void Quit();

  /*
   * Runs callback immediately in the loop thread.
   * It wakes up the loop, and run the cb. 
   * If in the same loop thread, cb is run within the function.
   * Safe to call from other threads. // 在其它线程里调用也是安全的。
   * 到loop线程去调用cb函数。   
   */
  void RunInLoop(Functor cb);

  /*
   * Queues callback in the loop thread.
   * Runs after finish pooling. 
   * Safe to call from other threads. // 在其它线程里调用也是安全的。
   * 把cb回调加到m_pendingFunctors队列中，并唤醒loop线程
   */
  void QueueInLoop(Functor cb);

  ///定时器相关
  // Runs callback at 'time'. Safe to call from other threads.    // 在time里调用cb. 在其它线程里调用也是安全的。
  TimerId RunAt(TimeStamp time, TimerCallback cb);
  // Runs callback after @c delay seconds. Safe to call from other threads. // 延迟delay秒后调用cb。在其它线程里调用也是安全的。 
  TimerId RunAfter(double delay, TimerCallback cb);
  // Runs callback every @c interval seconds. Safe to call from other threads.  // 间隔interval秒调用cb. 在其它线程里调用也是安全的。
  TimerId RunEvery(double interval, TimerCallback cb);
  // Cancels the timer. Safe to call from other threads.          // 取消定时器. 其它线程里调用也是安全的。
  void Cancel(TimerId timerId);

  ///internal usage   // 内部使用
  void Wakeup();                                                  // 往m_wakeupFd中写入8字节，以此唤醒该Loop线程
  void UpdateChannel(Channel* channel);                           // 调用m_poller->UpdateChannel(channel);
  void RemoveChannel(Channel* channel);                           // 调用m_poller->RemoveChannel(channel);
  bool HasChannel(Channel* channel);                              // 调用m_poller->HasChannel(channel);
  
  void AssertInLoopThread()                                       // 断言当前线程是该EventLoop所属线程
  {
    if (!IsInLoopThread())
    {
      abortNotInLoopThread();
    }
  }

  void setContext(const std::any& context) { m_context = context; }
  const std::any& getContext() const { return m_context; }
  std::any* getMutableContext() { return &m_context; }
  
  bool IsInLoopThread() const { return m_threadId == CurrentThread::tid(); }
  bool getEventHandling() const { return m_eventHandling; }
  TimeStamp getPollReturnTime() const { return m_pollReturnTime; }
  SQWORD getIteration() const { return m_iteration; }
  size_t getQueueSize() const;                                    // 调用MutexLockGuard, 获取m_pendingFunctors的大小

  static EventLoop* getEventLoopOfCurrentThread();

protected:
  void doPendingFunctors();                                       // 执行m_pendingFunctors中的函数

private:
  void abortNotInLoopThread();
  void handleRead();  // waked up                                 // 从m_wakeupFd中读入8个字节
  void printActiveChannels() const; // DEBUG

protected:
  bool m_looping; /* atomic */                                    // 是否正在loop()中运行
  std::atomic<bool> m_quit;                                       // loop()中的停止条件，在quit()中设置
  SQWORD m_iteration;                                             // 第几次循环

private:
  typedef std::vector<Channel*> ChannelList;

  bool m_eventHandling; /* atomic */                              // 是否正在loop()处理事件，即正在调用Channel::handleEvent(1)
  bool m_callingPendingFunctors; /* atomic */                     // 是否正在doPendingFunctors()中运行，即正在执行m_pendingFunctors中的函数
  const pid_t m_threadId;                                         // 线程id
  TimeStamp m_pollReturnTime;                                     // m_poller->poll(2)返回的时间
  std::unique_ptr<Poller> m_poller;
  std::unique_ptr<TimerQueue> m_timerQueue;                       // runAt(2)和runEvery(2)添加的定时器队列
  SDWORD m_wakeupFd;  // 一个能被用户应用程序用于时间等待唤醒机制的eventfd对象，用来唤醒该loop线程
  // unlike in TimerQueue, which is an internal class, we don't expose Channel to client.
  std::unique_ptr<Channel> m_wakeupChannel;                       // 唤醒的Channel
  std::any m_context;                                             // 
  // scratch variables
  ChannelList m_activeChannels;                                   // poll出来需要处理的Channels
  Channel* m_currentActiveChannel;                                // 当前运行的Channel
  mutable MutexLock m_mutex;
  std::vector<Functor> m_pendingFunctors GUARDED_BY(m_mutex);     // 保存即将执行的回调 
};

} // namespace net
} // namespace muduo

