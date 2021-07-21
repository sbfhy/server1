#pragma once

#include "src/base/common/singleton.h"
#include "src/base/mgr/mgr_base.h"

#include <memory>
#include <map>

namespace google {
namespace protobuf {

class Service;

}  // namespace protobuf
}  // namespace google

typedef std::shared_ptr<::google::protobuf::Service> ServicePtr;
typedef std::map<std::string, ServicePtr> ServiceMap;

class MgrMessage : public Singleton<MgrMessage>
                 , public MgrBase
{
public:
    virtual void Wake();

    const ServiceMap* GetService() { return &m_mapServices; }

private:
    // NOT thread safe, must call before RpcServer::start().
    void registerService();
    void registerService(ServicePtr);

private:
    ServiceMap m_mapServices;
};
