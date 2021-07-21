#pragma once

#include "src/gameserver/scene/assets.h"

class Scene
{
public:
    virtual ~Scene();

private:
    Assets m_assets;
};
