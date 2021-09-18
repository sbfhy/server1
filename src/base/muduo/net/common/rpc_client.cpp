#include "muduo/net/common/rpc_client.h"

#include "muduo/net/protorpc/RpcChannel.h"
#include "message/common/rpc.pb.h"

using namespace std::placeholders;


RpcClient::RpcClient(EventLoop* loop, const InetAddress& serverAddr)
    : m_loop(loop)
    , m_TcpClient(loop, serverAddr, "RpcClient")
    , m_RpcChannel(new RpcChannel)
{
  m_TcpClient.setConnectionCallback(
      std::bind(&RpcClient::onConnection, this, _1));
  m_TcpClient.setMessageCallback(
      std::bind(&RpcChannel::onMessage, get_pointer(m_RpcChannel), _1, _2, _3));
  // m_TcpClient.enableRetry();
}

RpcClient::~RpcClient()
{
    LOG_DEBUG << "loop地址: " << m_loop;
}

void RpcClient::Connect()
{
    m_TcpClient.connect();
}

void RpcClient::onConnection(const TcpConnectionPtr & conn)
{
    if (conn->IsConnected())
    {
        LOG_DEBUG << "主动连接成功" << conn->getPeerAddress().toIpPort();
        m_RpcChannel->setConnection(conn);
    }
}

void RpcClient::Send(const CMD::RpcMessage& rpcMsg)
{
    if (m_RpcChannel)
    {
        m_RpcChannel->Send(rpcMsg);
    }
}