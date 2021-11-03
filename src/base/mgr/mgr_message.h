#pragma once

#include "define/define_service.h"
#include "src/base/common/singleton.h"
#include "muduo/base/common/atomic.h"
#include "muduo/base/common/mutex.h"

namespace google {
namespace protobuf {

class Message;
typedef ::std::shared_ptr<Message> MessagePtr;
class ServiceDescriptor;

}   // namespace google {
}   // namespace protobuf

class MgrMessage : public Singleton<MgrMessage>
{
public:
    virtual void Wake() override;

    const ServicePtr GetServicePtr(ENUM::EServiceType) const;
    const SServiceInfo *GetServiceInfo(const ::google::protobuf::Descriptor *requestDesc) const;
    const ::google::protobuf::ServiceDescriptor* GetServiceDescriptor(ENUM::EServiceType) const;
    const ::google::protobuf::MethodDescriptor* GetMethodDescriptor(ENUM::EServiceType, int methodIdx) const;
    
    QWORD AddDelayResponse();

private:
    void registerService();
    void getServiceFromTo(const std::string& serviceTypeName, ENUM::EServerType &from, ENUM::EServerType &to);

private:
    TArrayService m_arrayService;
    TMapDescriptor2ServiceInfo m_mapRequest2ServiceInfo;

    // delay response,    FIXME: 非线程安全，只能在主线程中访问  
    // QWORD m_delayId;                                // delay自增Id
    // TMapQWORD2DelayInfo m_mapDelayId2DelayInfo;     // <delayId, DelayInfo>
public:
    SDelayResponse* NewDelayResponse(const SRpcChannelMethodArgs& args);
    void DoDelayResponse(QWORD delayResponseId);
protected:
    // delay response
    muduo::AtomicInt64 m_delayResponseId;
    muduo::MutexLock m_delayResponseMutex;
    std::map<QWORD, SDelayResponse> m_delayResponse;
};
