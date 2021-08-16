#pragma once

#include "muduo/base/define/define_service.h"
#include "src/base/common/singleton.h"

namespace google {
namespace protobuf {

class Message;
typedef ::std::shared_ptr<Message> MessagePtr;

}   // namespace google {
}   // namespace protobuf

class MgrMessage : public Singleton<MgrMessage>
{
public:
    virtual void Wake() override;
    virtual void Init() override;

    const ServicePtr GetServicePtr(ENUM::EServiceType) const;
    const SServiceInfo *GetServiceInfo(const ::google::protobuf::Descriptor *requestDesc) const;

private:
    void registerService();

private:
    TArrayService m_arrayService;
    TMapDescriptor2ServiceInfo m_mapRequest2ServiceInfo;
};
