#include "rpcchannel_gate.h"

#include "src/base/mgr/mgr_message.h"
#include "muduo/base/common/logging.h"
#include "muduo/net/common/tcp_connection.h"
#include "message/common/rpc.pb.h"
#include "service/service_include.pb.h"

void RpcChannelGate::serviceHandleRequestMsg(const RpcMessage& message)    // Service处理request消息
{
    if (message.to() == ENUM::ESERVERTYPE_GATESERVER)
    {
        RpcChannel::serviceHandleRequestMsg(message);
        return ;
    }


    // ErrorCode errorCode = ERR_NO_ERROR;
    // auto funcErrorCode = [&]()
    // {
    //     if (errorCode != ERR_NO_ERROR) // 告诉请求方处理request时出现错误
    //     {
    //         RpcMessage response;
    //         response.set_type(MSGTYPE_RESPONSE);
    //         response.set_id(message.id());
    //         response.set_error(errorCode);
    //         codec_.send(conn_, response);
    //     }
    // };

    // ServicePtr service = MgrMessage::Instance().GetServicePtr(message.service());
    // if (!service)
    // {
    //     errorCode = ERR_NO_SERVICE;
    //     return funcErrorCode();
    // }
    // const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
    // const google::protobuf::MethodDescriptor *method = desc->method(message.method());
    // if (!method)
    // {
    //     errorCode = ERR_NO_METHOD;
    //     return funcErrorCode();
    // }

    // // FIXME: can we move deserialization to other thread?
    // ::google::protobuf::MessagePtr request(service->GetRequestPrototype(method).New());
    // if (!request->ParseFromString(message.request()))
    // {
    //     errorCode = ERR_WRONG_PROTO;
    //     return funcErrorCode();
    // }
    // // {LDBG("M_NET") << request->ShortDebugString();}

    // int64_t id = message.id(); (void)id;
    // ::google::protobuf::MessagePtr response;

    // // 如果response类型是EmptyResponse，就不发回包
    // if (&service->GetResponsePrototype(method) != &CMD::EmptyResponse::default_instance())
    // {
    //     response = ::google::protobuf::MessagePtr(service->GetResponsePrototype(method).New()); 
    // }

    // // 调用处理函数
    // service->CallMethod(method, request, response);

    // if (response)                               // FIXME: delay response
    // {
    //     doneCallbackInIoLoop(response, id);     // 发送回包
    // }

    // funcErrorCode();
}

void RpcChannelGate::stubHandleResponseMsg(const RpcMessage& message)      // Stub处理response消息
{
    if (message.from() == ENUM::ESERVERTYPE_GATESERVER)
    {
        RpcChannel::stubHandleResponseMsg(message);
        return ;
    }


//     if (message.response() == "")
//         return ;
//     int64_t id = message.id();

//     OutstandingCall out;
//     bool found = false;

//     {
//         MutexLockGuard lock(mutex_);
//         std::map<int64_t, OutstandingCall>::iterator it = outstandings_.find(id);
//         if (it != outstandings_.end())
//         {
//             out = it->second;
//             outstandings_.erase(it);
//             found = true;
//         }
//         else
//         {
// #ifndef NDEBUG
//             LOG_WARN << "Size " << outstandings_.size();
//             for (it = outstandings_.begin(); it != outstandings_.end(); ++it)
//             {
//                 LOG_WARN << "id " << it->first;
//             }
// #endif
//         }
//     }

//     if (!found)
//     {
//         LOG_WARN << "[处理response-没找到对应id] " << message.ShortDebugString();
//         return ;
//     }
//     if (conn_ && conn_->GetLoop())
//     {
//         conn_->GetLoop()->Cancel(out.timerId);  // 删除超时回调的定时器
//     }

//     const ServicePtr pService = MgrMessage::Instance().GetServicePtr(out.serviceType);
//     if (!pService || !pService->GetDescriptor())
//     {
//         LOG_WARN << "[处理response-没找到service] " << message.ShortDebugString();
//         return ;
//     }
//     const auto methodDesc = pService->GetDescriptor()->method(out.methodIndex);
//     if (!methodDesc) return ;

//     // FIXME: can we move deserialization to other thread?
//     ::google::protobuf::MessagePtr response(
//             pService->GetResponsePrototype(methodDesc).New());
//     response->ParseFromString(message.response());
//     pService->DoneCallback(methodDesc, out.request, response);  // 调用Stub中的回调
}

