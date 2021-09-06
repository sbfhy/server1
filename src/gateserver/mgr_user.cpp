#include "mgr_user.h"

#include "muduo/base/common/logging.h"
#include "user.h"

bool MgrUser::UserLogin(QWORD accid)
{
    auto itFind = m_mapUser.find(accid);
    if (itFind == m_mapUser.end()) 
    {
        {LOG_DEBUG << "重复登录，accid:" << accid;}
        return false;
    }
    m_mapUser[accid] = std::make_shared<User>(accid);
    return true;
}

