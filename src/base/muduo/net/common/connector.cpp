#include "muduo/net/common/connector.h"
#include "muduo/base/common/logging.h"
#include "define/define_new.h"
#include "muduo/net/common/channel.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/sockets_ops.h"
#include <errno.h>

using namespace muduo;
using namespace muduo::net;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
  : m_loop(loop)
  , m_serverAddr(serverAddr)
  , m_connect(false)
  , m_state(kDisconnected)
  , m_retryDelayMs(kInitRetryDelayMs)
{
  LOG_DEBUG << "ctor[" << this << "]";
}

Connector::~Connector()
{
  LOG_DEBUG << "dtor[" << this << "]";
  assert(!m_channel);
}

void Connector::start()
{
  m_connect = true;
  m_loop->RunInLoop(
                    std::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}

void Connector::startInLoop()
{
  m_loop->AssertInLoopThread();
  assert(m_state == kDisconnected);
  if (m_connect)
  {
    connect();
  }
  else
  {
    LOG_DEBUG << "do not connect";
  }
}

void Connector::stop()
{
  m_connect = false;
  m_loop->QueueInLoop(
                      std::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
  // FIXME: cancel timer
}

void Connector::stopInLoop()
{
  m_loop->AssertInLoopThread();
  if (m_state == kConnecting)
  {
    setState(kDisconnected);
    int sockfd = removeAndResetChannel();
    retry(sockfd);
  }
}

void Connector::connect()
{
  int sockfd = sockets::createNonblockingOrDie(m_serverAddr.family());
  int ret = sockets::connect(sockfd, m_serverAddr.getSockAddr());
  int savedErrno = (ret == 0) ? 0 : errno;
  switch (savedErrno)
  {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
      connecting(sockfd);
      break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
      retry(sockfd);
      break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
      LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      break;

    default:
      LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      break;
  }
}

void Connector::restart()
{
  m_loop->AssertInLoopThread();
  setState(kDisconnected);
  m_retryDelayMs = kInitRetryDelayMs;
  m_connect = true;
  startInLoop();
}

void Connector::connecting(int sockfd)
{
  setState(kConnecting);
  assert(!m_channel);
  m_channel.reset(NEW Channel(m_loop, sockfd));
  m_channel->setWriteCallback(std::bind(&Connector::handleWrite, this)); // FIXME: unsafe
  m_channel->setErrorCallback(std::bind(&Connector::handleError, this)); // FIXME: unsafe
  // m_channel->tie(shared_from_this()); is not working, as m_channel is not managed by shared_ptr
  m_channel->EnableWriting();
}

int Connector::removeAndResetChannel()
{
  m_channel->DisableAll();
  m_channel->Remove();
  int sockfd = m_channel->getFd();
  // Can't reset m_channel here, because we are inside Channel::handleEvent
  m_loop->QueueInLoop(std::bind(&Connector::resetChannel, this)); // FIXME: unsafe
  return sockfd;
}

void Connector::resetChannel()
{
  m_channel.reset();
}

void Connector::handleWrite()
{
  LOG_TRACE << "Connector::handleWrite " << m_state;
  if (m_state == kConnecting)
  {
    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    if (err)
    {
      LOG_WARN << "Connector::handleWrite - SO_ERROR = " << err << " " << strerror_tl(err);
      retry(sockfd);
    }
    else if (sockets::isSelfConnect(sockfd))
    {
      LOG_WARN << "Connector::handleWrite - Self connect";
      retry(sockfd);
    }
    else
    {
      setState(kConnected);
      if (m_connect)
      {
        m_newConnectionCallback(sockfd);
      }
      else
      {
        sockets::close(sockfd);
      }
    }
  }
  else
  {
    // what happened?
    assert(m_state == kDisconnected);
  }
}

void Connector::handleError()
{
  LOG_ERROR << "Connector::handleError state=" << m_state;
  if (m_state == kConnecting)
  {
    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
    retry(sockfd);
  }
}

void Connector::retry(int sockfd)
{
  sockets::close(sockfd);
  setState(kDisconnected);
  if (m_connect)
  {
    LOG_INFO << "Connector::retry - Retry connecting to " << m_serverAddr.toIpPort()
             << " in " << m_retryDelayMs << " milliseconds. ";
    m_loop->RunAfter(m_retryDelayMs/1000.0,
                    std::bind(&Connector::startInLoop, shared_from_this()));
    m_retryDelayMs = std::min(m_retryDelayMs * 2, kMaxRetryDelayMs);
  }
  else
  {
    LOG_DEBUG << "do not connect";
  }
}

