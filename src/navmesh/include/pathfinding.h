#pragma once

#include <memory>

class PathFinding 
{
public:
    PathFinding();
    virtual ~PathFinding();

    bool Init(const std::string& filepath);


private:
    std::shared_ptr<class InputGeom> m_geom;
    std::shared_ptr<class Sample_TempObstacles> m_sample;
};
