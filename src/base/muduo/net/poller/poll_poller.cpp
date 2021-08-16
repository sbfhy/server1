#include "muduo/net/poller/poll_poller.h"
#include "muduo/net/common/channel.h"
#include "muduo/base/common/logging.h"
#include "muduo/base/define/define_types.h"
#include <asm-generic/errno-base.h>
#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/cdefs.h>

using namespace muduo;
using namespace muduo::net;

PollPoller::PollPoller(EventLoop* loop)
  : Poller(loop)
{
}

PollPoller::~PollPoller() = default;

TimeStamp PollPoller::poll(SDWORD timeoutMs, ChannelList* activeChannels)
{
  // XXX pollfds_ shouldn't change
  SDWORD numEvents = ::poll(&*m_pollfds.begin(), m_pollfds.size(), timeoutMs);
  SDWORD savedErrno = errno;
  TimeStamp now(TimeStamp::now());
  if (numEvents > 0)
  {
    LOG_TRACE << numEvents << " events happened";
    fillActiveChannels(numEvents, activeChannels);
  }
  else if (numEvents == 0)
  {
    LOG_TRACE << " nothing happened";
  }
  else
  {
    if (savedErrno != EINTR)
    {
      errno = savedErrno;
      LOG_SYSERR << "PollPoller::poll()";
    }
  }
  return now;
}

void PollPoller::fillActiveChannels(SDWORD numEvents, ChannelList* activeChannels) const
{
  for (PollFdList::const_iterator pfd = m_pollfds.begin(); 
       pfd != m_pollfds.end() && numEvents > 0; ++ pfd)
  {
    if (pfd->revents > 0)
    {
      -- numEvents;
      ChannelMap::const_iterator ch = m_channels.find(pfd->fd);
      assert(ch != m_channels.end());
      Channel* channel = ch->second;
      assert(channel->getFd() == pfd->fd);
      channel->setRevents(pfd->revents);
      activeChannels->push_back(channel);
    }
  }
}

void PollPoller::UpdateChannel(Channel* channel)
{
  Poller::AssertInLoopThread();
  LOG_TRACE << "fd = " << channel->getFd() << " events = " << channel->getEvents();
  if (channel->getIndex() < 0)
  {
    // a new one, add to pollfds_ 
    assert(m_channels.find(channel->getFd()) == m_channels.end());
    struct pollfd pfd;
    pfd.fd = channel->getFd();
    pfd.events = static_cast<SWORD>(channel->getEvents());
    pfd.revents = 0;
    m_pollfds.push_back(pfd);
    SDWORD idx = static_cast<SDWORD>(m_pollfds.size()) - 1;
    channel->setIndex(idx);
    m_channels[pfd.fd] = channel;
  }
  else
  {
    // update existing one
    assert(m_channels.find(channel->getFd()) != m_channels.end());
    assert(m_channels[channel->getFd()] == channel);
    SDWORD idx = channel->getIndex();
    assert(0 <= idx && idx < static_cast<SDWORD>(m_pollfds.size()));
    struct pollfd& pfd = m_pollfds[idx];
    assert(pfd.fd == channel->getFd() || pfd.fd == -channel->getFd()-1);
    pfd.fd = channel->getFd();
    pfd.events = static_cast<SWORD>(channel->getEvents());
    pfd.revents = 0;
    if (channel->IsNoneEvent())
    {
      pfd.fd = -channel->getFd() - 1;     // 删除，屏蔽掉这个fd
    }
  }
}

void PollPoller::RemoveChannel(Channel* channel)
{
  Poller::AssertInLoopThread();
  LOG_TRACE << "fd = " << channel->getFd();
  assert(m_channels.find(channel->getFd()) != m_channels.end());
  assert(m_channels[channel->getFd()] == channel);
  assert(channel->IsNoneEvent());
  SDWORD idx = channel->getIndex();
  assert(0 <= idx && idx < static_cast<SDWORD>(m_pollfds.size()));
  const struct pollfd& pfd = m_pollfds[idx]; (void)pfd;
  assert(pfd.fd == -channel->getFd()-1 && pfd.events == channel->getEvents());
  size_t n = m_channels.erase(channel->getFd());
  assert(n == 1); (void)n;
  if (implicit_cast<size_t>(idx) == m_pollfds.size() - 1)
  {
    m_pollfds.pop_back();
  }
  else
  {
    SDWORD channelAtEnd = m_pollfds.back().fd;
    iter_swap(m_pollfds.begin() + idx, m_pollfds.end() - 1);
    if (channelAtEnd < 0)
    {
      channelAtEnd = -channelAtEnd - 1;
    }
    m_channels[channelAtEnd]->setIndex(idx);
    m_pollfds.pop_back();
  }
}

