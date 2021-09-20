#pragma once

#include "define/define_variable.h"

#include <memory>

namespace google {
namespace protobuf {

class Message;           // message.h
typedef ::std::shared_ptr<Message> MessagePtr;

} // namespace protobuf
} // namespace google

class User
{
public:
    User(QWORD accid) : m_accid(accid) {}
    virtual ~User() {}

    void Send(const ::google::protobuf::MessagePtr& request);

private:
    QWORD m_accid {0};
};

typedef std::shared_ptr<User> UserPtr;

