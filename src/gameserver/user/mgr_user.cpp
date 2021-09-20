#include "src/gameserver/user/mgr_user.h"

#include "user.h"
#include "muduo/base/common/logging.h"

bool MgrUser::UserSignIn(QWORD accid)
{
    auto itFind = m_mapUser.find(accid);
    if (itFind != m_mapUser.end()) 
    {
        {LOG_DEBUG << "重复登录，accid:" << accid;}
        return false;
    }
    m_mapUser[accid] = std::make_shared<User>(accid);
    LOG_DEBUG << "用户上线, accid:" << accid;
    return true;
}

void MgrUser::UserSignOut(QWORD accid)
{
    m_mapUser.erase(accid);
    LOG_DEBUG << "用户下线, accid:" << accid;
}

UserPtr MgrUser::GetUser(QWORD accid)
{
    auto itFind = m_mapUser.find(accid);
    if (itFind == m_mapUser.end()) 
        return nullptr;
    return itFind->second;
}