#pragma once 

#include "muduo/net/common/tcp_client.h"
#include "muduo/base/common/logging.h"

using namespace muduo::net;

namespace muduo {
namespace net {
    class RpcChannel;
    typedef std::shared_ptr<RpcChannel> RpcChannelPtr;
}   // namespace net
}   // namespace muduo

namespace CMD {
    class RpcMessage;
}


class RpcClient : muduo::noncopyable
{
public:
    RpcClient(EventLoop* loop, const InetAddress& serverAddr);
    virtual ~RpcClient();

    void Connect();
    void Send(const CMD::RpcMessage& rpcMsg);
    void Send(const ::google::protobuf::MessagePtr& request);
    void SetConnectionSucceedCb(std::function<void ()> cb) { m_connectionSucceedCb = std::move(cb); }

private:
    void onConnection(const TcpConnectionPtr& conn);

private:
    EventLoop* m_loop;
    TcpClient m_TcpClient;
    RpcChannelPtr m_RpcChannel;
    std::function<void ()>  m_connectionSucceedCb;      // 连接成功后的回调
};
