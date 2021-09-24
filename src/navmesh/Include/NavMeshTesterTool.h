//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef NAVMESHTESTERTOOL_H
#define NAVMESHTESTERTOOL_H

#include "Sample.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

// 左侧Tools选择Test Navmesh时，就是对应这个类
class NavMeshTesterTool : public SampleTool
{
protected:
    Sample* m_sample;

    dtNavMesh* m_navMesh;
    dtNavMeshQuery* m_navQuery;

    dtQueryFilter m_filter;                 // 过滤

    dtStatus m_pathFindStatus;

    enum ToolMode                           // Test Navmesh的类型
    {
        TOOLMODE_PATHFIND_FOLLOW,           // 查找从起始多边形到终点多边形的路径
        TOOLMODE_PATHFIND_STRAIGHT,         // 查找从起点到终点的直线平滑路径
        TOOLMODE_PATHFIND_SLICED,           // 切片寻路
        TOOLMODE_RAYCAST,                   // 起点往终点打一条射线，遇到障碍物停下
        TOOLMODE_DISTANCE_TO_WALL,          // 显示起点到最近墙壁的距离
        TOOLMODE_FIND_POLYS_IN_CIRCLE,      // 在以起点为中心，起点到终点为半径的圆内，找出所有多边形
        TOOLMODE_FIND_POLYS_IN_SHAPE,       // 在长方形内，找出所有多边形
        TOOLMODE_FIND_LOCAL_NEIGHBOURHOOD,  // 找出起点周围的多边形
    };

    ToolMode m_toolMode;

    int m_straightPathOptions;

    static const int MAX_POLYS = 256;
    static const int MAX_SMOOTH = 2048;

    dtPolyRef m_startRef;
    dtPolyRef m_endRef;
    dtPolyRef m_polys[MAX_POLYS];
    dtPolyRef m_parent[MAX_POLYS];
    int m_npolys;
    float m_straightPath[MAX_POLYS * 3];
    unsigned char m_straightPathFlags[MAX_POLYS];
    dtPolyRef m_straightPathPolys[MAX_POLYS];
    int m_nstraightPath;
    float m_polyPickExt[3];
    float m_smoothPath[MAX_SMOOTH * 3];
    int m_nsmoothPath;
    float m_queryPoly[4 * 3];

    static const int MAX_RAND_POINTS = 64;
    float m_randPoints[MAX_RAND_POINTS * 3];
    int m_nrandPoints;
    bool m_randPointsInCircle;

    float m_spos[3];                        // 起点
    float m_epos[3];                        // 终点
    float m_hitPos[3];
    float m_hitNormal[3];
    bool m_hitResult;
    float m_distanceToWall;
    float m_neighbourhoodRadius;
    float m_randomRadius;
    bool m_sposSet;
    bool m_eposSet;

    int m_pathIterNum;
    dtPolyRef m_pathIterPolys[MAX_POLYS];
    int m_pathIterPolyCount;
    float m_prevIterPos[3], m_iterPos[3], m_steerPos[3], m_targetPos[3];

    static const int MAX_STEER_POINTS = 10;
    float m_steerPoints[MAX_STEER_POINTS * 3];
    int m_steerPointCount;

public:
    NavMeshTesterTool();

    virtual int type() { return TOOL_NAVMESH_TESTER; }
    virtual void init(Sample* sample);
    virtual void reset();
    // virtual void handleMenu();
    // virtual void handleClick(const float* s, const float* p, bool shift); // 点击事件
    virtual void handleToggle();
    virtual void handleStep();
    virtual void handleUpdate(const float dt);
    // virtual void handleRender();
    // virtual void handleRenderOverlay(double* proj, double* model, int* view);

    // 具体寻路过程
    void recalc();                                                                       // 重新计算路径
    // void drawAgent(const float* pos, float r, float h, float c, const unsigned int col); // 绘制路径
};

#endif // NAVMESHTESTERTOOL_H