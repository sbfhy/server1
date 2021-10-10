#pragma once

#include "src/base/common/singleton.h"
#include "src/navmesh/include/pathfinding.h"

#include <memory>

class MgrPathFindingConfig : public Singleton<MgrPathFindingConfig>
{
public:
    MgrPathFindingConfig();
    virtual ~MgrPathFindingConfig();

    virtual bool Init();

private:
    PathFinding m_pathFindingCapital;

};
