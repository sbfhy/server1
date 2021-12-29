#include "src/gameserver/entity/entity_dynamic.h"

#include "src/gameserver/user/user.h"
#include "src/gameserver/scene/scene.h"

EntityDynamic::~EntityDynamic()
{

}

bool EntityDynamic::EnterScene(ScenePtr pScene)
{
    setScene(pScene);
    pScene->AddEntity(shared_from_this());

    onEnterScene();
    return true;
}

void EntityDynamic::LeaveScene()
{
    auto m_scene = GetScene();
    if (m_scene)
    {
        onLeaveScene();
        m_scene->DelEntity(shared_from_this());
        resetScene();
    }
}

void EntityDynamic::onEnterScene()
{

}

void EntityDynamic::onLeaveScene()
{

}

void EntityDynamic::SetUser(UserPtr user)
{
    m_wUser = user; 
    if (!user) return ;
    m_accid = user->GetAccid();
}

void EntityDynamic::Send(const ::google::protobuf::MessagePtr& request)
{ 
    auto m_user = GetUser(); 
    if (!m_user) return ;
    m_user->Send(request); 
}