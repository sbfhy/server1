#pragma once

#include <memory>
#include <map>
#include <array>
#include <string>

#include "service/service_enum.pb.h"

namespace google {
namespace protobuf {

class Service;
class Descriptor;
class Message;

}  // namespace protobuf
}  // namespace google

namespace muduo {
namespace net {

class Service;

} // namespace net
} // namespace muduo

typedef std::shared_ptr<muduo::net::Service> ServicePtr;
// typedef std::map<std::string, ServicePtr> ServiceMap;
typedef std::array<ServicePtr, ENUM::SERVICETYPE_MAX> TArrayService;

struct SServiceInfo
{
    ENUM::EServiceType serviceType = ENUM::SERVICETYPE_MIN;
    int methodIndex = -1;
};
typedef std::map<const ::google::protobuf::Descriptor*, SServiceInfo> TMapDescriptor2ServiceInfo;
