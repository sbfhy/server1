#pragma once 

#include "common/singleton.h"
#include "define/define_variable.h"

#include "user.h"

#include <map>
#include <memory>

class xEntry;
class IVarList;

namespace CMD {
    class RpcMessage;
}   // namespace CMD


class MgrUser : public Singleton<MgrUser>
{
public:
    virtual bool Init() override;

    bool UserSignIn(QWORD accid, RpcChannelPtr rpcChannelPtr);
    bool CheckUserRpcChannel(QWORD accid, RpcChannelPtr rpcChannelPtr);
    void UserSignOut(xEntry* sender, const IVarList& varList);
    
    void Send(const CMD::RpcMessage& rpcMsg);

private:
    std::map<QWORD, std::shared_ptr<class User>> m_mapUser; // <accid, User>
};
