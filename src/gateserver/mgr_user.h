#pragma once 

#include "common/singleton.h"
#include "muduo/base/define/define_variable.h"

#include "user.h"

#include <map>
#include <memory>

class MgrUser : public Singleton<MgrUser>
{
public:
    bool UserLogin(QWORD accid);

private:
    std::map<QWORD, std::shared_ptr<class User>> m_mapUser; // <accid, User>
};
