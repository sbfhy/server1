#pragma once

#include "noncopyable.h"

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

namespace muduo
{
///
/// TCP networking.
///
namespace net
{

class InetAddress;

///
/// Wrapper of socket file descriptor.
///
/// It closes the sockfd when desctructs.
/// It's thread safe, all operations are delagated to OS.
class Socket : noncopyable
{
 public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd)
  { }

  // Socket(Socket&&) // move constructor in C++11
  virtual ~Socket();

  int fd() const { return sockfd_; }
  // return true if success.
  bool getTcpInfo(struct tcp_info*) const;
  bool getTcpInfoString(char* buf, int len) const;

  /// abort if address in use
  void bindAddress(const InetAddress& localaddr);
  /// abort if address in use
  void listen();

  /// On success, returns a non-negative integer that is
  /// a descriptor for the accepted socket, which has been
  /// set to non-blocking and close-on-exec. *peeraddr is assigned.
  /// On error, -1 is returned, and *peeraddr is untouched.
  int accept(InetAddress* peeraddr);

  void shutdownWrite();

  ///
  /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
  ///
  void setTcpNoDelay(bool on);

  /*
   * Enable/disable SO_REUSEADDR   设置套接字选项SO_REUSEADDR。
   * 如果开启该选项，对于端口bind，如果这个地址/端口处于TIME_WAIT，也可bind成功。即可以使用还处于TIME_WAIT状态的端口
   */
  void setReuseAddr(bool on);

  /*
   * Enable/disable SO_REUSEPORT
   * 如果开启SO_REUSEPORT选项，对于多核cpu，允许在同一个<ip, port>对上运行多个相同服务器，即支持多个进程或者线程绑定到同一端口。
   * 内核会采用负载均衡的的方式分配客户端的连接请求给某一个服务器。
   * 如果不设置该选项，在海量连接时会有两个瓶颈，单线程listen和多线程accept锁竞争。
   * 解决的问题：
   *    允许多个套接字 bind()/listen() 同一个TCP/UDP端口
   *    每一个线程拥有自己的服务器套接字
   *    在服务器套接字上没有了锁的竞争，因为每个进程一个服务器套接字
   *    内核层面实现负载均衡
   *    安全层面，监听同一个端口的套接字只能位于同一个用户下面
   */
  void setReusePort(bool on);

  /*
   * Enable/disable SO_KEEPALIVE
   * 设置KEEP-ALIVE属性，如果客户端很久没有和服务器通讯，tcp会自动判断客户端是否还处于连接（类似心跳包）
   */
  void setKeepAlive(bool on);

 private:
  const int sockfd_;
};

}  // namespace net
}  // namespace muduo

