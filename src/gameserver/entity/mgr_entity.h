#pragma once

#include "src/base/common/singleton.h"
#include "src/gameserver/entity/define_entity.h"
#include "src/gameserver/scene/define_scene.h"
#include "src/gameserver/user/define_user.h"

class MgrEntity : public Singleton<MgrEntity>
{
public:
    virtual ~MgrEntity();

public:
    EntityDynamicPtr CreateEntityDynamic(ScenePtr, UserPtr);
private:

};
