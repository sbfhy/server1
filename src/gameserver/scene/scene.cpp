#include "src/gameserver/scene/scene.h"

#include "src/gameserver/entity/entity_dynamic.h"
#include "service/g2c_scene.pb.h"

Scene::~Scene()
{
}

void Scene::Send(const ::google::protobuf::MessagePtr& request)
{
    for (auto pEntity : m_setEntityDynamic)
    {
        if (!pEntity) continue;
        pEntity->Send(request);
    }
}

void Scene::AddEntity(EntityDynamicPtr pEntity)
{
    m_setEntityDynamic.insert(pEntity);
    G2C_CreatePlayerPawn(pEntity);
}

void Scene::DelEntity(EntityDynamicPtr pEntity)
{
    m_setEntityDynamic.erase(pEntity);
}

void Scene::G2C_CreatePlayerPawn(EntityDynamicPtr pEntity)
{
    CMD::G2C_CreatePlayerPawnArgPtr G2C_CreatePlayerPawnArg = std::make_shared<CMD::G2C_CreatePlayerPawnArg>();
    G2C_CreatePlayerPawnArg->set_accid(pEntity->GetAccid());
    Send(G2C_CreatePlayerPawnArg);
}
