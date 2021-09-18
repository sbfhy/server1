#pragma once

#include "muduo/base/common/threadpool.h"
#include "muduo/net/common/eventloop.h"
#include "server_args.h"
#include "service/service_enum.pb.h"

#include<memory>

using namespace muduo;
using namespace muduo::net;

namespace CMD {
    class RpcMessage;
}   // namespace CMD

namespace muduo {
namespace net {
    class RpcChannel;
}   // namespace net
}   // namespace muduo 
typedef std::shared_ptr<muduo::net::RpcChannel> RpcChannelPtr; 

class MgrBase;

class Server : public EventLoop
             , public std::enable_shared_from_this<Server>
{
public:
    Server(const ServerArgs& args);

    virtual void Loop() override;
    virtual void Start();
    virtual ENUM::EServerType GetServerType() const { return ENUM::ESERVERTYPE_MIN; }
    virtual void ForwardRpcMsg(const CMD::RpcMessage &message, RpcChannelPtr rpcChannelPtr) {}

    void Run();

protected:
    void addMgr(MgrBase* mgr);
    virtual void registerMgrs();

private:
    virtual void tick(QWORD usec);

    void registerSignal();
    void wake();
    void init();
    void listen();

private:
    ThreadPool m_threadPool;
    std::vector<MgrBase*> m_mgrs;
    DWORD m_warnFrameOffset = 50 * 1000;    // 帧耗时警告(微秒)
    ServerArgs m_args;
};

extern std::shared_ptr<Server> thisServer;
