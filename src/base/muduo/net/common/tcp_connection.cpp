#include "muduo/net/common/tcp_connection.h"
#include "muduo/base/common/logging.h"
#include "muduo/base/common/weak_callback.h"
#include "muduo/base/define/define_new.h"
#include "muduo/net/common/channel.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/socket.h"
#include "muduo/net/common/sockets_ops.h"
#
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>

using namespace muduo;
using namespace muduo::net;

void muduo::net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{
  LOG_TRACE << conn->getLocalAddress().toIpPort() << " -> "
            << conn->getPeerAddress().toIpPort() << " is "
            << (conn->IsConnected() ? "UP" : "DOWN");
  // do not call conn->forceClose(), because some users want to register message callback only.
}

void muduo::net::defaultMessageCallback(const TcpConnectionPtr&, Buffer* buf, TimeStamp)
{
  buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop* loop, const string& nameArg, SDWORD sockfd, 
                             const InetAddress& localAddr, const InetAddress& peerAddr)
  : m_loop(CHECK_NOTNULL(loop))
  , m_name(nameArg)
  , m_state(kConnecting)
  , m_reading(true)
  , m_socket(NEW Socket(sockfd))
  , m_channel(NEW Channel(loop, sockfd))
  , m_localAddr(localAddr)
  , m_peerAddr(peerAddr)
  , m_highWaterMark(64 * 1024 * 1024)
{
  m_channel->setReadCallback(
                             std::bind(&TcpConnection::handleRead, this, _1));
  m_channel->setWriteCallback(
                              std::bind(&TcpConnection::handleWrite, this));
  m_channel->setCloseCallback(
                              std::bind(&TcpConnection::handleClose, this));
  m_channel->setErrorCallback(
                              std::bind(&TcpConnection::handleError, this));
  LOG_DEBUG << "TcpConnection::ctor[" << m_name << "] at " << this << " fd = " << sockfd;
  m_socket->setKeepAlive(true); // 设置KEEP-ALIVE属性，如果客户端很久没有和服务器通讯，tcp会自动判断客户端是否还处于连接（类似心跳包）
}

TcpConnection::~TcpConnection()
{
  LOG_DEBUG << "TcpConnection::dtor[" << m_name << "] at " << this 
            << " fd=" << m_channel->getFd() << " state=" << stateToString();
  assert(m_state == kDisconnected);
}

bool TcpConnection::getTcpInfo(struct tcp_info* tcpi) const 
{
  return m_socket->getTcpInfo(tcpi);
}

string TcpConnection::getTcpInfoString() const 
{
  CHAR buf[1024];
  buf[0] = '\0';
  m_socket->getTcpInfoString(buf, sizeof buf);
  return buf;
}

void TcpConnection::send(const void* data, SDWORD len)
{
  send(StringPiece(static_cast<const CHAR*>(data), len));
}

void TcpConnection::send(const StringPiece& message)
{
  if (m_state == kConnected)
  {
    if (m_loop->IsInLoopThread())
    {
      sendInLoop(message);
    }
    else 
    {
      void (TcpConnection::*fp)(const StringPiece& message) = &TcpConnection::sendInLoop;
      m_loop->RunInLoop( 
                        std::bind(fp, this/*FIXME*/, string(message)));
    }
  }
}

// FIXME efficiency!!!
void TcpConnection::send(Buffer* buf)
{
  if (m_state == kConnected)
  {
    if (m_loop->IsInLoopThread())
    {
      sendInLoop(buf->peek(), buf->readableBytes());
      buf->retrieveAll();
    }
    else
    {
      void (TcpConnection::*fp)(const StringPiece& message) = &TcpConnection::sendInLoop;
      m_loop->RunInLoop(std::bind(fp, this/*FIXME*/, buf->retrieveAllAsString()));
    }
  }
}

void TcpConnection::sendInLoop(const StringPiece& message)
{
  sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len)
{
  m_loop->AssertInLoopThread();
  ssize_t nwrote = 0;
  size_t remaining = len;                                     // 还有多少需要写入的
  bool faultError = false;
  if (m_state == kDisconnected)
  {
    LOG_WARN << "disconnected, give up writing";
    return ;
  }
  // if no thing in output queue, try writing directly
  if (!m_channel->IsWriting() && m_outputBuffer.readableBytes() == 0)
  {
    nwrote = sockets::write(m_channel->getFd(), data, len);   // nwrote真正写入了多少
    if (nwrote >= 0)
    {
      remaining = len - nwrote;
      if (remaining == 0 && m_writeCompleteCallback)          // 写入完成
      {
        m_loop->QueueInLoop(std::bind(m_writeCompleteCallback, shared_from_this()));
      }
    }
    else // nwrote < 0, 报错
    {
      nwrote = 0;
      if (errno != EWOULDBLOCK)
      {
        LOG_SYSERR << "TcpConnection::sendInLoop";
        if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
        {
          faultError = true;
        }
      }
    }
  }

  assert(remaining <= len);
  if (!faultError && remaining > 0)                           // 还剩下一些没有写入完
  {
    size_t oldLen = m_outputBuffer.readableBytes();
    if (oldLen + remaining >= m_highWaterMark                 // 如果再写入remaining，会超过限制，就调用m_highWaterMarkCallback
        && oldLen < m_highWaterMark 
        && m_highWaterMarkCallback)
    {
      m_loop->QueueInLoop(std::bind(m_highWaterMarkCallback, shared_from_this(), oldLen + remaining));
    }
    m_outputBuffer.append(static_cast<const CHAR*>(data) + nwrote, remaining);  // 把剩下的填到输出缓存中
    if (!m_channel->IsWriting())
    {
      m_channel->EnableWriting();
    }
  }
}

void TcpConnection::shutdown()
{
  LOG_TRACE << "state:" << stateToString();
  // FIXME: use compare and swap
  if (m_state == kConnected)
  {
    setState(kDisconnecting);
    // FIXME: shared_from_this()? 
    m_loop->RunInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
  }
}

void TcpConnection::shutdownInLoop()
{
  m_loop->AssertInLoopThread();
  if (!m_channel->IsWriting())
  {
    // we are not writing
    m_socket->shutdownWrite();
  }
}

void TcpConnection::ForceClose()
{
  LOG_TRACE << "state:" << stateToString();
  // FIXME: use compare and swap
  if (m_state == kConnected || m_state == kDisconnecting)
  {
    setState(kDisconnecting);
    m_loop->QueueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
  }
}

void TcpConnection::ForceCloseWithDelay(DOUBLE seconds)
{
  if (m_state == kConnected || m_state == kDisconnecting)
  {
    setState(kDisconnecting);
    m_loop->RunAfter(seconds, makeWeakCallback(shared_from_this()
                                               , &TcpConnection::ForceClose));  // not forceCloseInLoop to avoid race condition
  }
}

void TcpConnection::forceCloseInLoop()
{
  LOG_TRACE << "state: " << stateToString();
  m_loop->AssertInLoopThread();
  if (m_state == kConnected || m_state == kDisconnecting)
  {
    // as if we received 0 byte in handleRead(); 
    handleClose();
  }
}

const CHAR* TcpConnection::stateToString() const 
{
  switch (m_state)
  {
  case kDisconnected:
    return "kDisconnected";
  case kConnecting:
    return "kConnecting";
  case kConnected:
    return "kConnected";
  case kDisconnecting:
    return "kDisconnecting";
  default:
    return "unknown state";
  }
}

void TcpConnection::setTcpNoDelay(bool on)
{
  m_socket->setTcpNoDelay(on);
}

void TcpConnection::StartRead()
{
  m_loop->RunInLoop(std::bind(&TcpConnection::startReadInLoop, this));
}

void TcpConnection::startReadInLoop()
{
  m_loop->AssertInLoopThread();
  if (!m_reading || !m_channel->IsReading())
  {
    m_channel->EnableReading();
    m_reading = true;
  }
}

void TcpConnection::StopRead()
{
  m_loop->RunInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
}

void TcpConnection::stopReadInLoop()
{
  m_loop->AssertInLoopThread();
  if (m_reading || m_channel->IsReading())
  {
    m_channel->DisableReading();
    m_reading = false;
  }
}

void TcpConnection::ConnectEstablished()
{
  m_loop->AssertInLoopThread();
  assert(m_state == kConnecting);
  setState(kConnected);
  m_channel->Tie(shared_from_this());
  m_channel->EnableReading();                 // 开启可读监听，同时将m_channel添加到poller中
  //m_channel->EnableET();                      // ET模式
  m_connectionCallback(shared_from_this());
}

void TcpConnection::ConnectDestroyed()
{
  LOG_TRACE << "Connection fd = " << m_channel->getFd() << ", state=" << stateToString();
  m_loop->AssertInLoopThread();
  if (m_state == kConnected)
  {
    setState(kDisconnected);
    m_channel->DisableAll();
    m_connectionCallback(shared_from_this());
  }
  m_channel->Remove();
}

void TcpConnection::handleRead(TimeStamp receiveTime)
{
  m_loop->AssertInLoopThread();
  SDWORD savedErrno = 0;
  ssize_t n = m_inputBuffer.readFd(m_channel->getFd(), &savedErrno);
  if (n > 0)
  {
    m_messageCallback(shared_from_this(), &m_inputBuffer, receiveTime);
  }
  else if (n == 0)
  {
    handleClose();
  }
  else
  {
    errno = savedErrno;
    LOG_SYSERR << "TcpConnection::handleRead";
    handleError();
  }
}

void TcpConnection::handleWrite()
{
  m_loop->AssertInLoopThread();
  if (m_channel->IsWriting())
  {
    ssize_t n = sockets::write(m_channel->getFd(), m_outputBuffer.peek(), m_outputBuffer.readableBytes());
    if (n > 0)
    {
      m_outputBuffer.retrieve(n);
      if (m_outputBuffer.readableBytes() == 0)
      {
        m_channel->DisableWriting();
        if (m_writeCompleteCallback)
        {
          m_loop->QueueInLoop(std::bind(m_writeCompleteCallback, shared_from_this()));
        }
        if (m_state == kDisconnecting)
        {
          shutdownInLoop();
        }
      }
    }
    else
    {
      LOG_SYSERR << "TcpConnection::handleWrite";
    }
  }
  else
  {
    LOG_TRACE << "Connection fd = " << m_channel->getFd() << " is down, no more writing";
  }
}

void TcpConnection::handleClose()
{
  m_loop->AssertInLoopThread();
  LOG_TRACE << "fd = " << m_channel->getFd() << " state = " << stateToString();
  assert(m_state == kConnected || m_state == kDisconnecting);
  // we don't close fd, leave it to dtor, so we can find leaks easily.
  setState(kDisconnected);
  m_channel->DisableAll();
  
  TcpConnectionPtr guardThis(shared_from_this()); // 这个时候引用计数为2，在TcpServer里还记录了一个
  m_connectionCallback(guardThis);
  
  /* 
   * must be the last line
   * 这个回调是TcpServer::removeConnection，再调用TcpServer::removeConnectionInLoop，会把TcpServer中记录的TcpConnection删除，引用计数减一
   * 这个时候如果直接返回，且当前这个函数线束，引用计数就变为0，这样会直接把这个TcpConnection销毁。
   * 所以在TcpServer::removeConnectionInLoop中bind了这个TcpConnection，引用计数还是1，直到调用到TcpConnection::ConnectDestroyed才会销毁。
   */
  m_closeCallback(guardThis);
}

void TcpConnection::handleError()
{
  SDWORD err = sockets::getSocketError(m_channel->getFd());
  LOG_ERROR << "TcpConnection::handleError [ " << m_name << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}

