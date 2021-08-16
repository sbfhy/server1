#pragma once 

#include "muduo/base/common/noncopyable.h"
#include "muduo/base/common/string_piece.h"
#include "muduo/base/define/define_types.h"
#include "muduo/net/common/callbacks.h"
#include "muduo/net/common/buffer.h"
#include "muduo/net/common/inet_address.h"
#include <any>
#include <memory>
#include <string>

using std::string;

namespace google {
namespace protobuf {

class Message;
typedef ::std::shared_ptr<Message> MessagePtr;

}   // protobuf
}   // google

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

namespace muduo
{
namespace net
{

class Channel;
class EventLoop;
class Socket;

/*
 * TCP connection, for both client and server usage.
 * This is an interface class, so don't expose too much details.
 * 封装Tcp操作，表示一个Tcp连接。
 * 使用shared_ptr管理生命周期。
  	1. 建立之初保存在TcpServer的connections_中，这个connections_是一个map，键是字符串类型，值就是shared_ptr<TcpConnection>类型，所以建立之初，TcpConnection对象的引用计数为1
		2. 创建TcpConnection后，TcpConnection为内部用于监听文件描述符sockfd的Channel传递保存着自己指针的shared_ptr，但是Channel中的智能指针是以weak_ptr的形式存在的（tie_），不增加引用计数，所以此时仍然是1
		3. 在TcpConnection处于连接创建的过程中未有shared_ptr的创建和销毁，所以仍然是1
		4. 客户端主动关闭连接后，服务器端的TcpConnection在handleClose函数中又创建了一个shared_ptr引用自身的局部变量，此时引用计数加一，变为2
		5. 紧接着调用TcpServer::removeConnectionInLoop函数，因为传入的参数是引用类型，不存在赋值，所以引用计数仍为2
		6. 在removeConnectionInLoop函数中，TcpServer将TcpConnection从自己的connections_（保存所有tcp连接的map）中删除，此时引用计数减一，变为1
		7. TcpServer通过std::bind绑定函数指针，将TcpConnection::connectDestroyed函数和TcpConnection对象绑定并放到EventLoop中等待调用，因为std::bind只能是赋值操作，所以引用计数加一，变为2
		8. 返回到handleClose函数中，函数返回，局部变量销毁，引用计数减一，变为1
		9. EventLoop从poll中返回，执行TcpConnection::connectDestroyed函数，做最后的清理工作，函数返回，绑定到这个函数上的TcpConnection对象指针也跟着销毁，引用计数减一，变为0
		10. 开始调用TcpConnection析构函数，TcpConnection销毁 
 */
class TcpConnection : private noncopyable
                      , public std::enable_shared_from_this<TcpConnection>
{
public:
  TcpConnection(EventLoop* loop, const string& name, SDWORD sockfd, const InetAddress& localAddr, const InetAddress& peerAddr);
  virtual ~TcpConnection();

  std::shared_ptr<EventLoop> getMainLoop() const { return m_mainLoopPtr; }
  void SetMainLoop(std::shared_ptr<EventLoop> mainLoopPtr) { m_mainLoopPtr = mainLoopPtr; }
  EventLoop* getLoop() const { return m_loop; }
  const string& getName() const { return m_name; }
  const InetAddress& getLocalAddress() const { return m_localAddr; }
  const InetAddress& getPeerAddress() const { return m_peerAddr; }
  bool IsConnected() const { return m_state == kConnected; }
  bool IsDisConnected() const { return m_state == kDisconnected; }
  bool getTcpInfo(struct tcp_info*) const;              // return true if success. 
  string getTcpInfoString() const;

  // 下面几个函数都是在loop中调用对应的私有函数
  void send(const void* message, SDWORD len);           // 在loop中调用sendInLoop(2)
  void send(const StringPiece& message);                // 
  void send(Buffer* message);                           // this one will swap data
  void shutdown();                                      // NOT thread safe, no simultaneous calling
  void ForceClose();                                    // 
  void ForceCloseWithDelay(DOUBLE seconds);               // 
  void StartRead();                                     // 
  void StopRead();                                      // 

  void setTcpNoDelay(bool on);                          // 
  bool IsReading() const { return m_reading; }          // NOT thread safe, may race with start/stopReadInLoop
  void setContext(const std::any& context) { m_context = context; }
  const std::any& getContext() const { return m_context; }
  std::any* getMutableContext() { return &m_context; }

  void setConnectionCallback(const ConnectionCallback& cb) { m_connectionCallback = cb; }
  void setMessageCallback(const MessageCallback& cb) { m_messageCallback = cb; }
  void setWriteCompleteCallback(const WriteCompleteCallback& cb) { m_writeCompleteCallback = cb; }
  void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
        { m_highWaterMarkCallback = cb; m_highWaterMark = highWaterMark; }
  /// Internal use only.  // 只在内部使用
  void setCloseCallback(const CloseCallback& cb) { m_closeCallback = cb; }

  /// Advanced interface
  Buffer* getInputBuffer() { return &m_inputBuffer; }
  Buffer* getOutputBuffer() { return &m_outputBuffer; }

  // called when TcpServer accepts a new connection.      // 连接建立
  void ConnectEstablished();  // should be called only once
 
	// called when TcpServer has removed me from its map.   
	// 走到这个函数时，这个TcpConnection已经从TcpServer中删除了，调用完这个函数，该对象就销毁了
  void ConnectDestroyed();    // should be called only once

private:
  enum EState { kDisconnected, kConnecting, kConnected, kDisconnecting }; // 状态枚举
  
  // 给m_channel在HandleEvent中回调用的
  void handleRead(TimeStamp receiveTime);                 // 
  void handleWrite();                                     // 
  void handleClose();                                     // 执行各自关闭，会调用m_connectionCallback和m_closeCallback
  void handleError();                                     // 

  // 下面几个函数都要在loop线程中调用
  void sendInLoop(const StringPiece& message);            // 发送消息，在loop线程中才可调用
  void sendInLoop(const void* message, size_t len);       // 发送消息
  void shutdownInLoop();                                  // 调用m_socket->shutdownWrite()
  void forceCloseInLoop();                                // 调用handleClose()
  void startReadInLoop();                                 // 调用m_channel->EnableReading()
  void stopReadInLoop();                                  // 调用m_channel->DisableReading()

  void setState(EState s) { 
    m_state = s; 
   }
  const CHAR* stateToString() const;

private:
  std::shared_ptr<EventLoop> m_mainLoopPtr{nullptr};                  // 主线程loop
  EventLoop* m_loop;                                                  // io线程loop
  const string m_name;
  EState m_state;   // FIXME: use atomic variable                     // 状态
  bool m_reading;                                                     // 是否正在读入
  // we don't expose those classes to client.
  std::unique_ptr<Socket> m_socket;                                   // 该套接字
  std::unique_ptr<Channel> m_channel;                                 // 用来管理该描述符
  const InetAddress m_localAddr;                                      // 本地ip和端口
  const InetAddress m_peerAddr;                                       // 连接另一端的地址
  ConnectionCallback m_connectionCallback;                            // 连接建立、断开的事件回调
  MessageCallback m_messageCallback;                                  // 在handleRead(1)读入数据的时候事件回调
  WriteCompleteCallback m_writeCompleteCallback;                      // 写入完成
  HighWaterMarkCallback m_highWaterMarkCallback;                      // 要写入的数据量超出了m_outputBuffer的限制时，执行高水位回调
  CloseCallback m_closeCallback;                                      // 关闭连接的事件回调，即TcpServer::removeConnection
  size_t m_highWaterMark;                                             // 高水位值，m_outputBuffer的写入长度限制
  Buffer m_inputBuffer;                                               // 输入缓冲区
  Buffer m_outputBuffer; // FIXME: use list<Buffer> as output buffer. // 输出缓冲区
  std::any m_context;                                                 // 上下文，RpcChannel
  // FIXME: creationTime_, lastReceiveTime_
  //        bytesReceived_, bytesSent_
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

} // namespace net
} // namespace muduo

