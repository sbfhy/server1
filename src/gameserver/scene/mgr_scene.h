#pragma once

#include "src/base/common/singleton.h"
#include "src/base/mgr/mgr_base.h"

#include <memory>

class MgrScene : public Singleton<MgrScene>
               , public MgrBase
{
public:
    MgrScene();
    virtual ~MgrScene();

private:
    std::unique_ptr<class Scene> m_pSceneCapital;
};
