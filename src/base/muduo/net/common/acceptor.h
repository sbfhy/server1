#pragma once 

#include "muduo/net/common/channel.h"
#include "muduo/net/common/socket.h"
#include <functional>

namespace muduo
{
namespace net
{

class EventLoop;
class InetAddress;

/*
 * Acceptor of incoming TCP connections.
 * 对TCP socket, bind, listen, accept的封装.
 * 将sockfd以Channel的形式注册到EventLoop的Poller中，检测到sockfd可读时，接收客户端.
 */
class Acceptor : private noncopyable
{
public:
  typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
  virtual ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb) { m_newConnectionCallback = cb; }
  bool IsListenning() const { return m_listenning; }
  void listen();                                      // 调用m_acceptSocket.listen()，转为监听套接字，同时将监听套接字添加到Poller中

private:
	/*
	 * 主逻辑，m_acceptChannel读事件的回调函数，当有客户端请求连接时执行。
	 * 		当有客户端尝试连接服务器时，监听套接字变为可读，epoll_wait/poll返回
	 * 		EventLoop处理激活队列中的Channel，调用对应的回调函数，当处理到m_acceptChannel读事件的时候回调到这里
	 * 会把监听套接字变为可读, 并调用m_acceptSocket.accept。
	 */
  void handleRead();

private:
  EventLoop* m_loop;
  Socket m_acceptSocket;                              // 监听套接字
  Channel m_acceptChannel;                            // 管理监听描述符的Channel
  NewConnectionCallback m_newConnectionCallback;      // accept到新的连接时的回调，这个回调是给用户设置的, 客户端套接字和地址作为参数传入
  bool m_listenning;                                  // m_acceptSocket是否在listen()

	/* 
	 * 参考[muduo网络库学习（五）服务器监听类Acceptor及Tcp连接TcpConnection的建立与关闭](https://blog.csdn.net/sinat_35261315/article/details/78343266)  
	 * Tcp连接建立的流程
	 *    1.服务器调用socket,bind,listen开启监听套接字监听客户端请求
	 *    2.客户端调用socket,connect连接到服务器
	 *    3.第一次握手客户端发送SYN请求分节（数据序列号）
	 *    4.服务器接收SYN后保存在本地然后发送自己的SYN分节（数据序列号）和ACK确认分节告知客户端已收到
	 *      同时开启第二次握手
	 *    5.客户端接收到服务器的SYN分节和ACK确认分节后保存在本地然后发送ACK确认分节告知服务器已收到
	 *      此时第二次握手完成，客户端connect返回
	 *      此时，tcp连接已经建立完成，客户端tcp状态转为ESTABLISHED，而在服务器端，新建的连接保存在内核tcp
	 *      连接的队列中，此时服务器端监听套接字变为可读，等待服务器调用accept函数取出这个连接
	 *    6.服务器接收到客户端发来的ACK确认分节，服务器端调用accept尝试找到一个空闲的文件描述符，然后
	 *      从内核tcp连接队列中取出第一个tcp连接，分配这个文件描述符用于这个tcp连接
	 *      此时服务器端tcp转为ESTABLISHED，三次握手完成，tcp连接建立
	 *      
	 * 服务器启动时占用的一个空闲文件描述符，/dev/null,作用是解决文件描述符耗尽的情况
	 * 原理如下：
	 *    当服务器端文件描述符耗尽，当客户端再次请求连接，服务器端由于没有可用文件描述符
	 *        会返回-1，同时errno为EMFILE，意为描述符到达hard limit，无可用描述符，此时服务器端
	 *        accept函数在获取一个空闲文件描述符时就已经失败，还没有从内核tcp连接队列中取出tcp连接
	 *        这会导致监听套接字一直可读，因为tcp连接队列中一直有客户端的连接请求
	 *        
	 *    所以服务器在启动时打开一个空闲描述符/dev/null(文件描述符)，先站着'坑‘，当出现上面
	 *        情况accept返回-1时，服务器暂时关闭idleFd_让出'坑'，此时就会多出一个空闲描述符
	 *        然后再次调用accept接收客户端请求，然后close接收后的客户端套接字，优雅的告诉
	 *        客户端关闭连接，然后再将'坑'占上
	 */
	int m_idleFd; 		// 占用/dev/null文件描述符，用于文件描述符耗尽时拒绝连接
};

}  // namespace net
}  // namespace muduo

