#include "src/base/mgr/mgr_message.h"

#include "muduo/base/common/logging.h"
#include "dynamic_object.h"
#include "src/base/mgr/mgr_dynamicfactory.h"
#include "service.h"
#include "muduo/net/protorpc/RpcChannel.h"

#include "service/service_enum.pb.h"
#include <google/protobuf/message.h>

void MgrMessage::Wake()
{
    registerService();
}

void MgrMessage::registerService()
{
    m_arrayService.fill(nullptr);
    m_mapRequest2ServiceInfo.clear();

    for (int serviceType = ENUM::SERVICETYPE_MIN + 1; serviceType < ENUM::SERVICETYPE_MAX; ++ serviceType)
    {
        auto funcParseService = [this, &serviceType](const std::string& serviceTypeName)
        {
            ServicePtr ptrService = ServicePtr( static_cast<muduo::net::Service*>(
                                                MgrDynamicFactory::Instance().Create(serviceTypeName)) );
            // if (ENUM::EServiceType_IsValid(serviceType) && !ptrService)
            // {
            //     {LDBG("M_NET") << serviceTypeName << " 未注册";}
            // }
            if (!ptrService) return;
            const auto* serviceDesc = ptrService->GetDescriptor();
            if (!serviceDesc) return;

            m_arrayService.at(serviceType) = ptrService;

            ENUM::EServerType from{ ENUM::ESERVERTYPE_MIN }, to{ ENUM::ESERVERTYPE_MIN };
            getServiceFromTo(ENUM::EServiceType_Name(serviceType), from, to);

            for (int i = 0; i < serviceDesc->method_count(); ++ i)
            {
                const auto* requestDesc = ptrService->GetRequestPrototype(serviceDesc->method(i)).GetDescriptor();
                if (!requestDesc) continue;
                m_mapRequest2ServiceInfo[requestDesc] = SServiceInfo {
                                                            static_cast<ENUM::EServiceType>(serviceType), 
                                                            i,
                                                            from,
                                                            to
                                                        };
                {LDBG("M_NET") << "[service-注册method]" << serviceTypeName << ", " << serviceDesc->method(i)->name() << ", " << from << ", " << to;}
            }
        };

        const std::string& EServiceTypeName = ENUM::EServiceType_Name(serviceType);
        funcParseService("RPC::" + EServiceTypeName);
        funcParseService("RPC::" + EServiceTypeName + "_Stub");
    }
}

const ServicePtr MgrMessage::GetServicePtr(ENUM::EServiceType serviceType) const
{
    if (!ENUM::EServiceType_IsValid(serviceType)) 
        return nullptr;
    return m_arrayService.at(serviceType);
}

const SServiceInfo *MgrMessage::GetServiceInfo(const ::google::protobuf::Descriptor *requestDesc) const
{
    const auto itFind = m_mapRequest2ServiceInfo.find(requestDesc);
    if (itFind == m_mapRequest2ServiceInfo.end()) 
        return nullptr;
    return &itFind->second;
}

const ::google::protobuf::ServiceDescriptor* MgrMessage::GetServiceDescriptor(ENUM::EServiceType serviceType) const
{
    const auto ptr = GetServicePtr(serviceType);
    if (!ptr) return nullptr;
    return ptr->GetDescriptor();
}

const ::google::protobuf::MethodDescriptor* MgrMessage::GetMethodDescriptor(ENUM::EServiceType serviceType, int methodIdx) const
{
    const auto serviceDesc = GetServiceDescriptor(serviceType);
    if (!serviceDesc) return nullptr;
    return serviceDesc->method(methodIdx);
}

void MgrMessage::getServiceFromTo(const std::string& serviceTypeName, ENUM::EServerType &from, ENUM::EServerType &to)
{
    static const std::map<char, ENUM::EServerType> s_mapChar2ServiceType = {
        {'C', ENUM::ESERVERTYPE_CLIENT},
        {'A', ENUM::ESERVERTYPE_GATESERVER},
        {'G', ENUM::ESERVERTYPE_GAMESERVER},
    };

    auto func = [](char c, ENUM::EServerType &type)
    {
        auto itFind = s_mapChar2ServiceType.find(c);
        if (itFind == s_mapChar2ServiceType.end()) return;
        type = itFind->second;
    };

    if (serviceTypeName.size() < 3) return;
    func(serviceTypeName[0], from);
    func(serviceTypeName[2], to);
}

SDelayResponse* MgrMessage::NewDelayResponse(const SRpcChannelMethodArgs& args)
{
    QWORD delayId = m_delayResponseId.incrementAndGet();
    SDelayResponse* ret = nullptr;
    {
    muduo::MutexLockGuard lock(m_delayResponseMutex);
    ret = &m_delayResponse[delayId];
    }
    ret->delayResponseId = delayId;
    ret->accid = args.accid;
    ret->msgId = args.msgId;
    ret->rpcChannelPtr = args.rpcChannelPtr;
    ret->from = args.from;
    return ret;
}

void MgrMessage::DoDelayResponse(QWORD delayResponseId)
{
    SDelayResponse delayResponse;
    {
    muduo::MutexLockGuard lock(m_delayResponseMutex);
    auto itFind = m_delayResponse.find(delayResponseId);
    if (itFind == m_delayResponse.end())
        return ;
    delayResponse = itFind->second;
    m_delayResponse.erase(itFind);
    }
    if (delayResponse.rpcChannelPtr)
    {
        delayResponse.rpcChannelPtr->DoneCallbackInIoLoop(delayResponse.response, 
                                                          delayResponse.msgId, 
                                                          delayResponse.accid, 
                                                          delayResponse.from);
    }
}
