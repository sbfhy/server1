#pragma once

#include "noncopyable.h"
#include <functional>
#include <memory>
#include <vector>
#include <string>

using std::string;

namespace muduo
{
namespace net
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : private noncopyable
{
public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg);
  virtual ~EventLoopThreadPool();

  void setThreadNum(int numThreads) { m_numThreads = numThreads; }

  void start(const ThreadInitCallback& cb = ThreadInitCallback());  // 开始运行线程池，并初始化m_numThreads个线程

  // valid after calling start()
  /// round-robin
  EventLoop* getNextLoop();                                         // 从线程池中获取一个可用的EventLoop

  /// with the same hash code, it will always return the same EventLoop
  EventLoop* getLoopForHash(size_t hashCode);                       // 获取第 (hashCode % m_loops.size()) 个EventLoop

  std::vector<EventLoop*> getAllLoops();

  bool IsStarted() const { return m_started; }

  const string& getName() const { return m_name; }

private:
  EventLoop* m_baseLoop;                                    // 所属的线程，即主线程
  string m_name;
  bool m_started;                                           // 线程池是否开始运行
  int m_numThreads;                                         // 总数
  int m_next;                                               // 下一次获取用哪个
  std::vector<std::unique_ptr<EventLoopThread>> m_threads;  // 线程队列
  std::vector<EventLoop*> m_loops;                          // EventLoop队列，和m_threads一一对应
};

}  // namespace net
}  // namespace muduo

