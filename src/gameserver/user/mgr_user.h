#pragma once

#include "src/base/common/singleton.h"

#include <memory>
#include <map>

class User;
typedef std::shared_ptr<User> UserPtr;

class MgrUser : public Singleton<MgrUser>
{
public:
    bool UserSignIn(QWORD accid);
    void UserSignOut(QWORD accid);

    UserPtr GetUser(QWORD accid);

private:
    std::map<QWORD, std::shared_ptr<User>> m_mapUser; // <accid, User>
};
