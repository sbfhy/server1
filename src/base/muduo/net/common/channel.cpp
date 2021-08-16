#include "muduo/net/common/channel.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/base/common/logging.h"
#include <memory>
#include <sstream>
#include <poll.h>
#include <sys/poll.h>
#include <sys/epoll.h>

using namespace muduo;
using namespace muduo::net;

/*
 * 事件类型 :
 * POLLIN              有数据可读。
 * POLLRDNORM          有普通数据可读。
 * POLLRDBAND          有优先数据可读。
 * POLLPRI             有紧迫数据可读。
 * POLLOUT             写数据不会导致阻塞。
 * POLLWRNORM          写普通数据不会导致阻塞。
 * POLLWRBAND          写优先数据不会导致阻塞。
 * POLLMSGSIGPOLL      消息可用。
 * POLLRDHUP           读入挂起事件，表示套接字另一边关闭了连接或关闭了写操作。
 * 此外，revents域中还可能返回下列事件：
 * POLLER              指定的文件描述符发生错误。
 * POLLHUP             指定的文件描述符挂起事件。
 * POLLNVAL            指定的文件描述符非法。
 * EPOLLET             设置ET模式
 */
const SDWORD Channel::kNoneEvent = 0;
const SDWORD Channel::kReadEvent = POLLIN | POLLPRI;
const SDWORD Channel::kWriteEvent = POLLOUT;
const SDWORD Channel::kEPollET = EPOLLET;

Channel::Channel(EventLoop* loop, SDWORD fd__)
  : m_loop(loop)
  , m_fd(fd__)
  , m_events(0)
  , m_revents(0)
  , m_index(-1)
  , m_logHup(true)
  , m_tied(false)
  , m_eventHandling(false)
  , m_addedToLoop(false)
{
}

Channel::~Channel()
{
  assert(!m_eventHandling);
  assert(!m_addedToLoop);
  if (m_loop->IsInLoopThread())
  {
    assert(!m_loop->HasChannel(this));
  }
}

void Channel::Tie(const std::shared_ptr<void>& obj)
{
  m_tie = obj;
  m_tied = true;
}

void Channel::update()
{
  m_addedToLoop = true;
  m_loop->UpdateChannel(this);
}

void Channel::Remove()
{
  assert(IsNoneEvent());
  m_addedToLoop = false;
  m_loop->RemoveChannel(this);
}

void Channel::HandleEvent(TimeStamp receiveTime)
{
  std::shared_ptr<void> guard;
  if (m_tied)
  {
    guard = m_tie.lock();
    if (guard)
    {
      handleEventWithGuard(receiveTime);
    }
  }
  else
  {
    handleEventWithGuard(receiveTime);
  }
}

void Channel::handleEventWithGuard(TimeStamp receiveTime)
{
  m_eventHandling = true;
  LOG_TRACE << ReventsToString();
  if ((m_revents & POLLHUP) && !(m_revents & POLLIN))
  {
    if (m_logHup)
    {
      LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLHUP";
    }
    if (m_closeCallback) m_closeCallback();
  }

  if (m_revents & POLLNVAL)
  {
    LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLNVAL";
  }

  if (m_revents & (POLLERR | POLLNVAL))
  {
    if (m_errorCallback) m_errorCallback();
  }
  if (m_revents & (POLLIN | POLLPRI | POLLRDHUP))
  {
    if (m_readCallback) m_readCallback(receiveTime);
  }
  if (m_revents & POLLOUT)
  {
    if (m_writeCallback) m_writeCallback();
  }
  m_eventHandling = false;
}

std::string Channel::ReventsToString() const 
{
  return eventsToString(m_fd, m_revents);
}

std::string Channel::EventsToString() const 
{
  return eventsToString(m_fd, m_events);
}

std::string Channel::eventsToString(SDWORD fd, SDWORD ev)
{
  std::ostringstream oss;
  oss << fd << ": ";
  if (ev & POLLIN)
    oss << "IN ";
  if (ev & POLLPRI)
    oss << "PRI";
  if (ev & POLLOUT)
    oss << "OUT ";
  if (ev & POLLHUP)
    oss << "HUP ";
  if (ev & POLLRDHUP)
    oss << "RDHUP ";
  if (ev & POLLERR)
    oss << "ERR ";
  if (ev & POLLNVAL)
    oss << "NVAL ";
  return oss.str();
}

