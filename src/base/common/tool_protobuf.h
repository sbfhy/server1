#include <string>

namespace google {
namespace protobuf {

class Message;

}  // namespace protobuf
}  // namespace google

class ToolProtobuf
{
public:
    static ::google::protobuf::Message* GetMessageByName(const std::string& name);
};
