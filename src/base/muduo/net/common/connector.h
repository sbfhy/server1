#pragma once

#include "noncopyable.h"
#include "muduo/net/common/inet_address.h"
#include <functional>
#include <memory>

namespace muduo
{
namespace net
{

class Channel;
class EventLoop;

/*
 *
 */
class Connector : private noncopyable
                , public std::enable_shared_from_this<Connector>
{
public:
  typedef std::function<void (int sockfd)> NewConnectionCallback;

  Connector(EventLoop* loop, const InetAddress& serverAddr);
  virtual ~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb) { m_newConnectionCallback = cb; }

  void start();     // can be called in any thread
  void restart();   // must be called in loop thread
  void stop();      // can be called in any thread

  const InetAddress& getServerAddress() const { return m_serverAddr; }

private:
  enum States { kDisconnected, kConnecting, kConnected };
  static const int kMaxRetryDelayMs = 30*1000;
  static const int kInitRetryDelayMs = 500;

  void setState(States s) { m_state = s; }
  void startInLoop();
  void stopInLoop();
  void connect();
  void connecting(int sockfd);
  void handleWrite();
  void handleError();
  void retry(int sockfd);
  int  removeAndResetChannel();
  void resetChannel();

private:
  EventLoop* m_loop;
  InetAddress m_serverAddr;                           // 需要连接的对端 ip和端口
  bool m_connect;   // atomic                         // 这个对象是否在运行中，start置为true, stop置为false
  States m_state;   // FIXME: use atomic variable     // 状态
  std::unique_ptr<Channel> m_channel;                 // 管理这个描述符的通道
  NewConnectionCallback m_newConnectionCallback;      // 新的连接建立时的事件回调
  int m_retryDelayMs;                                 // 尝试重连的时间间隔
};

}  // namespace net
}  // namespace muduo

