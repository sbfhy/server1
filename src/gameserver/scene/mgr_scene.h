#pragma once

#include "src/base/common/singleton.h"

#include <memory>

class MgrScene : public Singleton<MgrScene>
{
public:
    MgrScene();
    virtual ~MgrScene();

private:
    std::unique_ptr<class Scene> m_pSceneCapital;
};
