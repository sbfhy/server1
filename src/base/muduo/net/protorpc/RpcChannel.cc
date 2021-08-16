#include "muduo/net/protorpc/RpcChannel.h"

#include "muduo/base/common/logging.h"
#include "muduo/net/common/eventloop.h"
#include "muduo/net/common/buffer.h"
#include "muduo/net/common/tcp_connection.h"
#include "src/base/mgr/mgr_message.h"

#include "message/common/rpc.pb.h"
#include "service/service_include.pb.h"
#include <google/protobuf/descriptor.h>

using namespace muduo;
using namespace muduo::net;

static int test_down_pointer_cast()
{
    ::std::shared_ptr<::google::protobuf::Message> msg(new RpcMessage);
    ::std::shared_ptr<RpcMessage> rpc(::google::protobuf::down_pointer_cast<RpcMessage>(msg));
    assert(msg && rpc);
    if (!rpc)
    {
        abort();
    }
    return 0;
}

static int dummy __attribute__((unused)) = test_down_pointer_cast();

RpcChannel::RpcChannel()
    : codec_(std::bind(&RpcChannel::onRpcMessage, this, _1, _2, _3))
{
    LOG_INFO << "RpcChannel::ctor - " << this;
}

RpcChannel::RpcChannel(const TcpConnectionPtr &conn)
    : codec_(std::bind(&RpcChannel::onRpcMessage, this, _1, _2, _3))
    , conn_(conn)
{
    LOG_INFO << "RpcChannel::ctor - " << this;
}

RpcChannel::~RpcChannel()
{
    LOG_INFO << "RpcChannel::dtor - " << this;
}

void RpcChannel::Send(const ::google::protobuf::MessagePtr& request)
{
    // FIXME: can we move serialization to IO thread? 
    if (!request) return ;
    const SServiceInfo *serviceInfo = MgrMessage::Instance().GetServiceInfo(request->GetDescriptor());
    if (!serviceInfo) return ;

    RpcMessage message;
    message.set_type(MSGTYPE_REQUEST);
    int64_t id = id_.incrementAndGet();
    message.set_id(id);
    message.set_service(static_cast<ENUM::EServiceType>(static_cast<int>(serviceInfo->serviceType) - 1));
    message.set_method(serviceInfo->methodIndex);
    message.set_request(request->SerializeAsString());  // FIXME: error check

    {LDBG("M_NET") << message.ShortDebugString();}

    // FIXME: timeout
    OutstandingCall out = { request, serviceInfo->serviceType, serviceInfo->methodIndex}; 
    {
    MutexLockGuard lock(mutex_);
    outstandings_[id] = out;
    }
    codec_.send(conn_, message);
}

// Call the given method of the remote service.  The signature of this
// procedure looks the same as Service::CallMethod(), but the requirements
// are less strict in one important way:  the request and response objects
// need not be of any specific class as long as their descriptors are
// method->input_type() and method->output_type().
void RpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor *method,
                            const ::google::protobuf::MessagePtr &request,
                            const ::google::protobuf::MessagePtr &response)
{
}

void RpcChannel::onDisconnect()
{
    //FIXME:
}

void RpcChannel::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buf,
                           TimeStamp receiveTime)
{
    LOG_TRACE << "RpcChannel::onMessage " << buf->readableBytes();
    codec_.onMessage(conn, buf, receiveTime);
}

void RpcChannel::onRpcMessageInMainLoop(const TcpConnectionPtr &conn,
                                        const RpcMessagePtr &messagePtr,
                                        TimeStamp receiveTime)
{
    std::shared_ptr<EventLoop> mainLoopPtr = conn->getMainLoop();
    if (mainLoopPtr)
    {
        mainLoopPtr->RunInLoop(
            std::bind(&RpcChannel::onRpcMessage, this, conn, messagePtr, receiveTime));
    }
    else
    {
        onRpcMessage(conn, messagePtr, receiveTime);
    }
}

void RpcChannel::onRpcMessage(const TcpConnectionPtr &conn,
                              const RpcMessagePtr &messagePtr,
                              TimeStamp receiveTime)
{
    assert(conn == conn_);
    RpcMessage &message = *messagePtr;
    //   LDBG("M_NET") << message.DebugString();
    if (message.type() == MSGTYPE_RESPONSE)
    {
        solveResponseMsg(message);
    }
    else if (message.type() == MSGTYPE_REQUEST)
    {
        solveRequestMsg(message);
    }
    else if (message.type() == MSGTYPE_ERROR)
    {
        // FIXME:
    }
}

void RpcChannel::solveRequestMsg(const RpcMessage &message) // 处理request消息
{
    ErrorCode errorCode = ERR_NO_ERROR;
    auto funcErrorCode = [&]()
    {
        if (errorCode != ERR_NO_ERROR) // 告诉请求方处理request时出现错误
        {
            RpcMessage response;
            response.set_type(MSGTYPE_RESPONSE);
            response.set_id(message.id());
            response.set_error(errorCode);
            codec_.send(conn_, response);
        }
    };

    ServicePtr service = MgrMessage::Instance().GetServicePtr(message.service());
    if (!service)
    {
        errorCode = ERR_NO_SERVICE;
        return funcErrorCode();
    }
    const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
    const google::protobuf::MethodDescriptor *method = desc->method(message.method());
    if (!method)
    {
        errorCode = ERR_NO_METHOD;
        return funcErrorCode();
    }

    // FIXME: can we move deserialization to other thread?
    ::google::protobuf::MessagePtr request(service->GetRequestPrototype(method).New());
    if (!request->ParseFromString(message.request()))
    {
        errorCode = ERR_WRONG_PROTO;
        return funcErrorCode();
    }
    // {LDBG("M_NET") << request->ShortDebugString();}

    int64_t id = message.id(); (void)id;
    ::google::protobuf::MessagePtr response;

    // 如果response类型是EmptyResponse，就不发回包，即不执行doneCallback
    if (&service->GetResponsePrototype(method) != &CMD::EmptyResponse::default_instance())
    {
        response = ::google::protobuf::MessagePtr(service->GetResponsePrototype(method).New()); 
    }

    // 调用处理函数
    service->CallMethod(method, request, response);

    if (response)                               // FIXME: delay response
    {
        doneCallbackInIoLoop(response, id); // 发送回包
    }

    funcErrorCode();
}

void RpcChannel::solveResponseMsg(const RpcMessage &message)    // 处理response消息
{
    if (message.response() == "")
        return ;
    int64_t id = message.id();

    OutstandingCall out;
    bool found = false;

    {
        MutexLockGuard lock(mutex_);
        std::map<int64_t, OutstandingCall>::iterator it = outstandings_.find(id);
        if (it != outstandings_.end())
        {
            out = it->second;
            outstandings_.erase(it);
            found = true;
        }
        else
        {
#ifndef NDEBUG
            LOG_WARN << "Size " << outstandings_.size();
            for (it = outstandings_.begin(); it != outstandings_.end(); ++it)
            {
                LOG_WARN << "id " << it->first;
            }
#endif
        }
    }

    if (!found)
    {
        LOG_WARN << "[处理response-没找到对应id] " << message.ShortDebugString();
        return ;
    }

    const ServicePtr pService = MgrMessage::Instance().GetServicePtr(out.serviceType);
    if (!pService || !pService->GetDescriptor())
    {
        LOG_WARN << "[处理response-没找到service] " << message.ShortDebugString();
        return ;
    }
    const auto methodDesc = pService->GetDescriptor()->method(out.methodIndex);
    if (!methodDesc) return ;

    // FIXME: can we move deserialization to other thread?
    ::google::protobuf::MessagePtr response(
            pService->GetResponsePrototype(methodDesc).New());
    response->ParseFromString(message.response());
    pService->DoneCallback(methodDesc, out.request, response);
}

void RpcChannel::doneCallbackInIoLoop(::google::protobuf::MessagePtr response,
                                      int64_t id)
{
    EventLoop *pIoLoop = conn_->getLoop();
    if (pIoLoop)
    {
        pIoLoop->RunInLoop(
            std::bind(&RpcChannel::doneCallback, this, response, id));
    }
    else
    {
        doneCallback(response, id);
    }
}

void RpcChannel::doneCallback(::google::protobuf::MessagePtr response,
                              int64_t id)
{
    if (!response) return;
    // FIXME: can we move serialization to IO thread?
    RpcMessage message;
    message.set_type(MSGTYPE_RESPONSE);
    message.set_id(id);
    message.set_response(response->SerializeAsString()); // FIXME: error check
    codec_.send(conn_, message);
    LDBG("M_NET") << response->ShortDebugString();
}

