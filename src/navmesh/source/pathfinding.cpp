#include "pathfinding.h"

#include "Sample.h"
#include "Sample_TempObstacles.h"
#include "InputGeom.h"

#include "DetourNavMeshBuilder.h"

PathFinding::PathFinding()
    : m_geom(new InputGeom)
    , m_sample(new Sample_TempObstacles)
{
}

PathFinding::~PathFinding()
{
}

bool PathFinding::Init(const std::string& filepath)
{
    if (!m_geom 
        || !m_sample
        || !m_geom->load(nullptr, filepath)) 
        return false;
    
    m_sample->handleMeshChanged(m_geom.get());
    

    return true;
}
