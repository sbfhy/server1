#include "src/gameserver/entity/mgr_entity.h"

#include "src/gameserver/entity/entity_dynamic.h"

MgrEntity::~MgrEntity()
{

}

EntityDynamicPtr MgrEntity::CreateEntityDynamic(ScenePtr pScene, UserPtr pUser)
{
    EntityDynamicPtr pEntity = std::make_shared<EntityDynamic>();
    if (!pEntity) return nullptr;
    pEntity->SetUser(pUser);
    if (pScene && !pEntity->EnterScene(pScene))
    {
        return nullptr;
    }
    return pEntity;
}

