#include "mgr_pathfinding_config.h"

#include <string>

extern const std::string & G_GetProjectDir();

MgrPathFindingConfig::MgrPathFindingConfig()
{

}

MgrPathFindingConfig::~MgrPathFindingConfig()
{

}

bool MgrPathFindingConfig::Init()
{
    m_pathFindingCapital.Init(G_GetProjectDir() + "tables/maps/capital/capital.obj");

    return true;
}
