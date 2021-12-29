#pragma once

#include "src/base/common/singleton.h"
#include "src/gameserver/scene/define_scene.h"

#include <memory>

class MgrScene : public Singleton<MgrScene>
{
public:
    MgrScene();
    virtual ~MgrScene();

    virtual bool Init();

public:
    ScenePtr GetSceneCapital() const { return m_pSceneCapital; }
private:
    ScenePtr m_pSceneCapital;   // 主城


};
