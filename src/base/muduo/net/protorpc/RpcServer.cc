#include "muduo/net/protorpc/RpcServer.h"

#include "muduo/net/protorpc/RpcChannel.h"
#include "muduo/base/common/logging.h"
#include "src/gateserver/mgr_user.h"
#include "src/base/mgr/mgr_event_global.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

using namespace muduo;
using namespace muduo::net;

RpcServer::RpcServer(EventLoop* loop,
                     const InetAddress& listenAddr)
    : server_(loop, listenAddr, "RpcServer")
{
    server_.setConnectionCallback(
        std::bind(&RpcServer::onConnection, this, _1));
    //   server_.setMessageCallback(
    //       std::bind(&RpcServer::onMessage, this, _1, _2, _3));

      // registerService(&metaService_);
}

RpcServer::RpcServer(std::shared_ptr<EventLoop> mainLoop,
                     EventLoop* loop,
                     const InetAddress& listenAddr)
    : m_mainLoop(mainLoop)
    , server_(loop, listenAddr, "RpcServer")
{
    server_.setConnectionCallback(
        std::bind(&RpcServer::onConnection, this, _1));
}

void RpcServer::start()
{
    server_.start();
}

void RpcServer::onConnection(const TcpConnectionPtr& conn)
{
    LOG_INFO << "RpcServer - " << conn->getPeerAddress().toIpPort() << " -> "
        << conn->getLocalAddress().toIpPort() << " is "
        << (conn->IsConnected() ? "UP" : "DOWN");
    if (conn->IsConnected())      // 上线
    {
        RpcChannelPtr channel(new RpcChannel(conn));
        conn->setMessageCallback(
            std::bind(&RpcChannel::onMessage, get_pointer(channel), _1, _2, _3));
        conn->setContext(channel);
        conn->SetMainLoop(m_mainLoop);
    }
    else                          // 下线
    {
        RpcChannelPtr rpcChannelPtr = std::any_cast<RpcChannelPtr>(conn->getContext());
        if (rpcChannelPtr)
        {
            SEND_EVENT_GLOBAL_4(EG_EVENT_TYPE_SIGN_OUT, 
                                static_cast<xEntry*>(this), 
                                0, 
                                CVarList() << rpcChannelPtr->GetAccid());
        }

        conn->setContext(RpcChannelPtr());
        // FIXME:
    }
}
