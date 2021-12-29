#include "src/gameserver/user/user.h"

#include "src/gameserver/gameserver.h"
#include "src/gameserver/entity/mgr_entity.h"
#include "src/gameserver/entity/entity_dynamic.h"
#include "service/g2c_scene.pb.h"

User::~User()
{
    if (m_playerPawn)
    {
        m_playerPawn->LeaveScene();
    }
}

void User::Send(const ::google::protobuf::MessagePtr& request)
{
    thisServer->SendToGate(request, m_accid);
}

void User::GotoMap(ScenePtr pScene)
{
    G2C_EnterScene();
    if (m_playerPawn)
    {
        m_playerPawn->LeaveScene();
        m_playerPawn.reset();
    }
    SetScene(pScene);
}

void User::CreatePlayerPawn()
{
    auto m_scene = GetScene();
    m_playerPawn = MgrEntity::Instance().CreateEntityDynamic(m_scene, shared_from_this());
}

void User::G2C_EnterScene()
{
    CMD::G2C_EnterSceneArgPtr G2C_EnterSceneArgPtr = std::make_shared<CMD::G2C_EnterSceneArg>();
    G2C_EnterSceneArgPtr->set_mapid(1);     // TODO
    Send(G2C_EnterSceneArgPtr);
}

