#include "src/base/common/tool_protobuf.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

::google::protobuf::Message* ToolProtobuf::GetMessageByName(const std::string &name)
{
    auto descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name);
    if (!descriptor) return nullptr;
    auto prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
    if (!descriptor) return nullptr;
    return prototype->New();
}
