#include "src/gameserver/user/mgr_user.h"

#include "user.h"
#include "muduo/base/common/logging.h"

#include "service/g2c_scene.pb.h"

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

    CMD::G2C_EnterSceneArgPtr G2C_EnterSceneArgPtr = std::make_shared<CMD::G2C_EnterSceneArg>();
    G2C_EnterSceneArgPtr->set_mapid(1);
    m_mapUser[accid]->Send(G2C_EnterSceneArgPtr);

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