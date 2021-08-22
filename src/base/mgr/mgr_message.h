#pragma once

#include "muduo/base/define/define_service.h"
#include "src/base/common/singleton.h"

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
    virtual void Init() override;

    const ServicePtr GetServicePtr(ENUM::EServiceType) const;
    const SServiceInfo *GetServiceInfo(const ::google::protobuf::Descriptor *requestDesc) const;
    const ::google::protobuf::ServiceDescriptor* GetServiceDescriptor(ENUM::EServiceType) const;
    const ::google::protobuf::MethodDescriptor* GetMethodDescriptor(ENUM::EServiceType, int methodIdx) const;
    
    QWORD AddDelayResponse();

private:
    void registerService();

private:
    TArrayService m_arrayService;
    TMapDescriptor2ServiceInfo m_mapRequest2ServiceInfo;

    // delay response,    FIXME: 非线程安全，只能在主线程中访问  
    QWORD m_delayId;                                // delay自增Id
    TMapQWORD2DelayInfo m_mapDelayId2DelayInfo;     // <delayId, DelayInfo>
};
