#include "muduo/net/poller/epoll_poller.h"
#include "muduo/net/common/channel.h"
#include "muduo/base/common/logging.h"
#include "define/define_types.h"
#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

// On Linux, the constants of poll(2) and epoll(4) are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

namespace
{
const SDWORD kNew = -1;
const SDWORD kAdded = 1;
const SDWORD kDeleted = 2;
}

EPollPoller::EPollPoller(EventLoop* loop)
  : Poller(loop)
  , m_epollfd(::epoll_create1(EPOLL_CLOEXEC))     // 创建的m_epollfd会设置FD_CLOEXEC
  , m_events(kInitEventListSize)
{
  if (m_epollfd < 0)
  {
    LOG_SYSFATAL << "EPollPoller::EPollPoller";
  }
}

EPollPoller::~EPollPoller()
{
  ::close(m_epollfd);
}

TimeStamp EPollPoller::poll(SDWORD timeoutMs, ChannelList* activeChannels)
{
  LOG_TRACE << "fd total count " << m_channels.size();
  SDWORD numEvents = ::epoll_wait(m_epollfd, &*m_events.begin(), static_cast<SDWORD>(m_events.size()), timeoutMs);
  SDWORD savedErrno = errno;
  TimeStamp now(TimeStamp::now());
  if (numEvents > 0)
  {
    LOG_TRACE << numEvents << " events happened";
    fillActiveChannels(numEvents, activeChannels);
    if (implicit_cast<size_t>(numEvents) == m_events.size())
    {
      m_events.resize(m_events.size()*2);
    }
  }
  else if (numEvents == 0)
  {
    LOG_TRACE << "nothing happened";
  }
  else
  {
    // error happens, log uncommon ones
    if (savedErrno != EINTR)
    {
      errno = savedErrno;
      LOG_SYSERR << "EPollPoller::poll()";
    }
  }
  return now;
}

void EPollPoller::fillActiveChannels(SDWORD numEvents, ChannelList* activeChannels) const
{
  assert(implicit_cast<size_t>(numEvents) <= m_events.size());
  for (SDWORD i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(m_events[i].data.ptr);
#ifndef NDEBUG
    SDWORD fd = channel->getFd();
    ChannelMap::const_iterator it = m_channels.find(fd);
    assert(it != m_channels.end());
    assert(it->second == channel);
#endif
    channel->setRevents(m_events[i].events);
    activeChannels->push_back(channel);
  }
}

void EPollPoller::UpdateChannel(Channel* channel)
{
  Poller::AssertInLoopThread();
  const SDWORD index = channel->getIndex();
  LOG_TRACE << "fd = " << channel->getFd() << " events = " << channel->getEvents() << " index = " << index;
  if (index == kNew || index == kDeleted)
  {
    // a new one, add with EPOLL_CTL_ADD
    SDWORD fd = channel->getFd();
    if (index == kNew)
    {
      assert(m_channels.find(fd) == m_channels.end());
      m_channels[fd] = channel;
    }
    else // index == kDeleted
    {
      assert(m_channels.find(fd) != m_channels.end());
      assert(m_channels[fd] == channel);
    }

    channel->setIndex(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  else
  {
    // update existing one with EPOLL_CTL_MOD/DEL
    SDWORD fd = channel->getFd();  (void)fd;
    assert(m_channels.find(fd) != m_channels.end());
    assert(m_channels[fd] == channel);
    assert(index == kAdded);
    if (channel->IsNoneEvent())
    {
      update(EPOLL_CTL_DEL, channel);
      channel->setIndex(kDeleted);
    }
    else
    {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

void EPollPoller::RemoveChannel(Channel* channel)
{
  Poller::AssertInLoopThread();
  SDWORD fd = channel->getFd();
  LOG_TRACE << "fd = " << fd;
  assert(m_channels.find(fd) != m_channels.end());
  assert(m_channels[fd] == channel);
  assert(channel->IsNoneEvent());
  SDWORD index = channel->getIndex();
  assert(index == kAdded || index == kDeleted);
  size_t n = m_channels.erase(fd);  (void)n;
  assert(n == 1);

  if (index == kAdded)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->setIndex(kNew);
}

void EPollPoller::update(SDWORD operation, Channel* channel)
{
  struct epoll_event event;
  memZero(&event, sizeof event);
  event.events = channel->getEvents();
  event.data.ptr = channel;
  SDWORD fd = channel->getFd();
  LOG_TRACE << "epoll_ctl op = " << operationToString(operation)
            << " fd = " << fd << " event = { " << channel->EventsToString() << " }";
  if (::epoll_ctl(m_epollfd, operation, fd, &event) < 0)
  {
    if (operation == EPOLL_CTL_DEL)
    {
      LOG_SYSERR << "epoll_ctl op = " << operationToString(operation) << " fd = " << fd;
    }
    else
    {
      LOG_SYSFATAL << "epoll_ctl op = " << operationToString(operation) << " fd = " << fd;
    }
  }
}

const CHAR* EPollPoller::operationToString(SDWORD op)
{
  switch (op)
  {
    case EPOLL_CTL_ADD:
      return "ADD";
    case EPOLL_CTL_DEL:
      return "DEL";
    case EPOLL_CTL_MOD:
      return "MOD";
    default:
      assert(false && "ERROR op");
      return "Unknown Operation";
  }
}
