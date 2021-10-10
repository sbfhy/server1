// //
// // Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
// //
// // This software is provided 'as-is', without any express or implied
// // warranty.  In no event will the authors be held liable for any damages
// // arising from the use of this software.
// // Permission is granted to anyone to use this software for any purpose,
// // including commercial applications, and to alter it and redistribute it
// // freely, subject to the following restrictions:
// // 1. The origin of this software must not be misrepresented; you must not
// //    claim that you wrote the original software. If you use this software
// //    in a product, an acknowledgment in the product documentation would be
// //    appreciated but is not required.
// // 2. Altered source versions must be plainly marked as such, and must not be
// //    misrepresented as being the original software.
// // 3. This notice may not be removed or altered from any source distribution.
// //

// #define _USE_MATH_DEFINES
// #include <math.h>
// #include <stdio.h>
// #include <string.h>
// #include "SDL.h"
// #include "SDL_opengl.h"
// #include "imgui.h"
// #include "InputGeom.h"
// #include "Sample.h"
// #include "Sample_SoloMesh.h"
// #include "Recast.h"
// #include "RecastDebugDraw.h"
// #include "RecastDump.h"
// #include "DetourNavMesh.h"
// #include "DetourNavMeshBuilder.h"
// #include "DetourDebugDraw.h"
// #include "NavMeshTesterTool.h"
// #include "NavMeshPruneTool.h"
// #include "OffMeshConnectionTool.h"
// #include "ConvexVolumeTool.h"
// #include "CrowdTool.h"

// #ifdef WIN32
// #	define snprintf _snprintf
// #endif


// Sample_SoloMesh::Sample_SoloMesh() :
//     m_keepInterResults(true),
//     m_totalBuildTimeMs(0),
//     m_triareas(0),
//     m_solid(0),
//     m_chf(0),
//     m_cset(0),
//     m_pmesh(0),
//     m_dmesh(0),
//     m_drawMode(DRAWMODE_NAVMESH)
// {
//     setTool(new NavMeshTesterTool);
// }
        
// Sample_SoloMesh::~Sample_SoloMesh()
// {
//     cleanup();
// }
    
// void Sample_SoloMesh::cleanup()
// {
//     delete [] m_triareas;
//     m_triareas = 0;
//     rcFreeHeightField(m_solid);
//     m_solid = 0;
//     rcFreeCompactHeightfield(m_chf);
//     m_chf = 0;
//     rcFreeContourSet(m_cset);
//     m_cset = 0;
//     rcFreePolyMesh(m_pmesh);
//     m_pmesh = 0;
//     rcFreePolyMeshDetail(m_dmesh);
//     m_dmesh = 0;
//     dtFreeNavMesh(m_navMesh);
//     m_navMesh = 0;
// }

// void Sample_SoloMesh::handleSettings()
// {
//     Sample::handleCommonSettings();

//     if (imguiCheck("Keep Itermediate Results", m_keepInterResults))
//         m_keepInterResults = !m_keepInterResults;

//     imguiSeparator();

//     imguiIndent();
//     imguiIndent();

//     if (imguiButton("Save"))
//     {
//         Sample::saveAll("solo_navmesh.bin", m_navMesh);
//     }

//     if (imguiButton("Load"))
//     {
//         dtFreeNavMesh(m_navMesh);
//         m_navMesh = Sample::loadAll("solo_navmesh.bin");
//         m_navQuery->init(m_navMesh, 2048);
//     }

//     imguiUnindent();
//     imguiUnindent();
    
//     char msg[64];
//     snprintf(msg, 64, "Build Time: %.1fms", m_totalBuildTimeMs);
//     imguiLabel(msg);

//     imguiSeparator();
// }

// void Sample_SoloMesh::handleTools()
// {
//     int type = !m_tool ? TOOL_NONE : m_tool->type();
    
//     if (imguiCheck("Test Navmesh", type == TOOL_NAVMESH_TESTER))
//     {
//         setTool(new NavMeshTesterTool);
//     }
//     if (imguiCheck("Prune Navmesh", type == TOOL_NAVMESH_PRUNE))
//     {
//         setTool(new NavMeshPruneTool);
//     }
//     if (imguiCheck("Create Off-Mesh Connections", type == TOOL_OFFMESH_CONNECTION))
//     {
//         setTool(new OffMeshConnectionTool);
//     }
//     if (imguiCheck("Create Convex Volumes", type == TOOL_CONVEX_VOLUME))
//     {
//         setTool(new ConvexVolumeTool);
//     }
//     if (imguiCheck("Create Crowds", type == TOOL_CROWD))
//     {
//         setTool(new CrowdTool);
//     }
    
//     imguiSeparatorLine();

//     imguiIndent();

//     if (m_tool)
//         m_tool->handleMenu();

//     imguiUnindent();

// }

// void Sample_SoloMesh::handleDebugMode()
// {
//     // Check which modes are valid.
//     bool valid[MAX_DRAWMODE];
//     for (int i = 0; i < MAX_DRAWMODE; ++i)
//         valid[i] = false;

//     if (m_geom)
//     {
//         valid[DRAWMODE_NAVMESH] = m_navMesh != 0;
//         valid[DRAWMODE_NAVMESH_TRANS] = m_navMesh != 0;
//         valid[DRAWMODE_NAVMESH_BVTREE] = m_navMesh != 0;
//         valid[DRAWMODE_NAVMESH_NODES] = m_navQuery != 0;
//         valid[DRAWMODE_NAVMESH_INVIS] = m_navMesh != 0;
//         valid[DRAWMODE_MESH] = true;
//         valid[DRAWMODE_VOXELS] = m_solid != 0;
//         valid[DRAWMODE_VOXELS_WALKABLE] = m_solid != 0;
//         valid[DRAWMODE_COMPACT] = m_chf != 0;
//         valid[DRAWMODE_COMPACT_DISTANCE] = m_chf != 0;
//         valid[DRAWMODE_COMPACT_REGIONS] = m_chf != 0;
//         valid[DRAWMODE_REGION_CONNECTIONS] = m_cset != 0;
//         valid[DRAWMODE_RAW_CONTOURS] = m_cset != 0;
//         valid[DRAWMODE_BOTH_CONTOURS] = m_cset != 0;
//         valid[DRAWMODE_CONTOURS] = m_cset != 0;
//         valid[DRAWMODE_POLYMESH] = m_pmesh != 0;
//         valid[DRAWMODE_POLYMESH_DETAIL] = m_dmesh != 0;
//     }
    
//     int unavail = 0;
//     for (int i = 0; i < MAX_DRAWMODE; ++i)
//         if (!valid[i]) unavail++;

//     if (unavail == MAX_DRAWMODE)
//         return;

//     imguiLabel("Draw");
//     if (imguiCheck("Input Mesh", m_drawMode == DRAWMODE_MESH, valid[DRAWMODE_MESH]))
//         m_drawMode = DRAWMODE_MESH;
//     if (imguiCheck("Navmesh", m_drawMode == DRAWMODE_NAVMESH, valid[DRAWMODE_NAVMESH]))
//         m_drawMode = DRAWMODE_NAVMESH;
//     if (imguiCheck("Navmesh Invis", m_drawMode == DRAWMODE_NAVMESH_INVIS, valid[DRAWMODE_NAVMESH_INVIS]))
//         m_drawMode = DRAWMODE_NAVMESH_INVIS;
//     if (imguiCheck("Navmesh Trans", m_drawMode == DRAWMODE_NAVMESH_TRANS, valid[DRAWMODE_NAVMESH_TRANS]))
//         m_drawMode = DRAWMODE_NAVMESH_TRANS;
//     if (imguiCheck("Navmesh BVTree", m_drawMode == DRAWMODE_NAVMESH_BVTREE, valid[DRAWMODE_NAVMESH_BVTREE]))
//         m_drawMode = DRAWMODE_NAVMESH_BVTREE;
//     if (imguiCheck("Navmesh Nodes", m_drawMode == DRAWMODE_NAVMESH_NODES, valid[DRAWMODE_NAVMESH_NODES]))
//         m_drawMode = DRAWMODE_NAVMESH_NODES;
//     if (imguiCheck("Voxels", m_drawMode == DRAWMODE_VOXELS, valid[DRAWMODE_VOXELS]))
//         m_drawMode = DRAWMODE_VOXELS;
//     if (imguiCheck("Walkable Voxels", m_drawMode == DRAWMODE_VOXELS_WALKABLE, valid[DRAWMODE_VOXELS_WALKABLE]))
//         m_drawMode = DRAWMODE_VOXELS_WALKABLE;
//     if (imguiCheck("Compact", m_drawMode == DRAWMODE_COMPACT, valid[DRAWMODE_COMPACT]))
//         m_drawMode = DRAWMODE_COMPACT;
//     if (imguiCheck("Compact Distance", m_drawMode == DRAWMODE_COMPACT_DISTANCE, valid[DRAWMODE_COMPACT_DISTANCE]))
//         m_drawMode = DRAWMODE_COMPACT_DISTANCE;
//     if (imguiCheck("Compact Regions", m_drawMode == DRAWMODE_COMPACT_REGIONS, valid[DRAWMODE_COMPACT_REGIONS]))
//         m_drawMode = DRAWMODE_COMPACT_REGIONS;
//     if (imguiCheck("Region Connections", m_drawMode == DRAWMODE_REGION_CONNECTIONS, valid[DRAWMODE_REGION_CONNECTIONS]))
//         m_drawMode = DRAWMODE_REGION_CONNECTIONS;
//     if (imguiCheck("Raw Contours", m_drawMode == DRAWMODE_RAW_CONTOURS, valid[DRAWMODE_RAW_CONTOURS]))
//         m_drawMode = DRAWMODE_RAW_CONTOURS;
//     if (imguiCheck("Both Contours", m_drawMode == DRAWMODE_BOTH_CONTOURS, valid[DRAWMODE_BOTH_CONTOURS]))
//         m_drawMode = DRAWMODE_BOTH_CONTOURS;
//     if (imguiCheck("Contours", m_drawMode == DRAWMODE_CONTOURS, valid[DRAWMODE_CONTOURS]))
//         m_drawMode = DRAWMODE_CONTOURS;
//     if (imguiCheck("Poly Mesh", m_drawMode == DRAWMODE_POLYMESH, valid[DRAWMODE_POLYMESH]))
//         m_drawMode = DRAWMODE_POLYMESH;
//     if (imguiCheck("Poly Mesh Detail", m_drawMode == DRAWMODE_POLYMESH_DETAIL, valid[DRAWMODE_POLYMESH_DETAIL]))
//         m_drawMode = DRAWMODE_POLYMESH_DETAIL;
        
//     if (unavail)
//     {
//         imguiValue("Tick 'Keep Itermediate Results'");
//         imguiValue("to see more debug mode options.");
//     }
// }

// void Sample_SoloMesh::handleRender()
// {
//     if (!m_geom || !m_geom->getMesh())
//         return;
    
//     glEnable(GL_FOG);
//     glDepthMask(GL_TRUE);

//     const float texScale = 1.0f / (m_cellSize * 10.0f);
    
//     if (m_drawMode != DRAWMODE_NAVMESH_TRANS)
//     {
//         // Draw mesh
//         duDebugDrawTriMeshSlope(&m_dd, m_geom->getMesh()->getVerts(), m_geom->getMesh()->getVertCount(),
//                                 m_geom->getMesh()->getTris(), m_geom->getMesh()->getNormals(), m_geom->getMesh()->getTriCount(),
//                                 m_agentMaxSlope, texScale);
//         m_geom->drawOffMeshConnections(&m_dd);
//     }
    
//     glDisable(GL_FOG);
//     glDepthMask(GL_FALSE);

//     // Draw bounds
//     const float* bmin = m_geom->getNavMeshBoundsMin();
//     const float* bmax = m_geom->getNavMeshBoundsMax();
//     duDebugDrawBoxWire(&m_dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], duRGBA(255,255,255,128), 1.0f);
//     m_dd.begin(DU_DRAW_POINTS, 5.0f);
//     m_dd.vertex(bmin[0],bmin[1],bmin[2],duRGBA(255,255,255,128));
//     m_dd.end();
    
//     if (m_navMesh && m_navQuery &&
//         (m_drawMode == DRAWMODE_NAVMESH ||
//         m_drawMode == DRAWMODE_NAVMESH_TRANS ||
//         m_drawMode == DRAWMODE_NAVMESH_BVTREE ||
//          m_drawMode == DRAWMODE_NAVMESH_NODES ||
//         m_drawMode == DRAWMODE_NAVMESH_INVIS))
//     {
//         if (m_drawMode != DRAWMODE_NAVMESH_INVIS)
//             duDebugDrawNavMeshWithClosedList(&m_dd, *m_navMesh, *m_navQuery, m_navMeshDrawFlags);
//         if (m_drawMode == DRAWMODE_NAVMESH_BVTREE)
//             duDebugDrawNavMeshBVTree(&m_dd, *m_navMesh);
//         if (m_drawMode == DRAWMODE_NAVMESH_NODES)
//             duDebugDrawNavMeshNodes(&m_dd, *m_navQuery);
//         duDebugDrawNavMeshPolysWithFlags(&m_dd, *m_navMesh, SAMPLE_POLYFLAGS_DISABLED, duRGBA(0,0,0,128));
//     }
        
//     glDepthMask(GL_TRUE);
    
//     if (m_chf && m_drawMode == DRAWMODE_COMPACT)
//         duDebugDrawCompactHeightfieldSolid(&m_dd, *m_chf);

//     if (m_chf && m_drawMode == DRAWMODE_COMPACT_DISTANCE)
//         duDebugDrawCompactHeightfieldDistance(&m_dd, *m_chf);
//     if (m_chf && m_drawMode == DRAWMODE_COMPACT_REGIONS)
//         duDebugDrawCompactHeightfieldRegions(&m_dd, *m_chf);
//     if (m_solid && m_drawMode == DRAWMODE_VOXELS)
//     {
//         glEnable(GL_FOG);
//         duDebugDrawHeightfieldSolid(&m_dd, *m_solid);
//         glDisable(GL_FOG);
//     }
//     if (m_solid && m_drawMode == DRAWMODE_VOXELS_WALKABLE)
//     {
//         glEnable(GL_FOG);
//         duDebugDrawHeightfieldWalkable(&m_dd, *m_solid);
//         glDisable(GL_FOG);
//     }
//     if (m_cset && m_drawMode == DRAWMODE_RAW_CONTOURS)
//     {
//         glDepthMask(GL_FALSE);
//         duDebugDrawRawContours(&m_dd, *m_cset);
//         glDepthMask(GL_TRUE);
//     }
//     if (m_cset && m_drawMode == DRAWMODE_BOTH_CONTOURS)
//     {
//         glDepthMask(GL_FALSE);
//         duDebugDrawRawContours(&m_dd, *m_cset, 0.5f);
//         duDebugDrawContours(&m_dd, *m_cset);
//         glDepthMask(GL_TRUE);
//     }
//     if (m_cset && m_drawMode == DRAWMODE_CONTOURS)
//     {
//         glDepthMask(GL_FALSE);
//         duDebugDrawContours(&m_dd, *m_cset);
//         glDepthMask(GL_TRUE);
//     }
//     if (m_chf && m_cset && m_drawMode == DRAWMODE_REGION_CONNECTIONS)
//     {
//         duDebugDrawCompactHeightfieldRegions(&m_dd, *m_chf);
            
//         glDepthMask(GL_FALSE);
//         duDebugDrawRegionConnections(&m_dd, *m_cset);
//         glDepthMask(GL_TRUE);
//     }
//     if (m_pmesh && m_drawMode == DRAWMODE_POLYMESH)
//     {
//         glDepthMask(GL_FALSE);
//         duDebugDrawPolyMesh(&m_dd, *m_pmesh);
//         glDepthMask(GL_TRUE);
//     }
//     if (m_dmesh && m_drawMode == DRAWMODE_POLYMESH_DETAIL)
//     {
//         glDepthMask(GL_FALSE);
//         duDebugDrawPolyMeshDetail(&m_dd, *m_dmesh);
//         glDepthMask(GL_TRUE);
//     }
    
//     m_geom->drawConvexVolumes(&m_dd);

//     if (m_tool)
//         m_tool->handleRender();
//     renderToolStates();

//     glDepthMask(GL_TRUE);
// }

// void Sample_SoloMesh::handleRenderOverlay(double* proj, double* model, int* view)
// {
//     if (m_tool)
//         m_tool->handleRenderOverlay(proj, model, view);
//     renderOverlayToolStates(proj, model, view);
// }

// void Sample_SoloMesh::handleMeshChanged(class InputGeom* geom)
// {
//     Sample::handleMeshChanged(geom);

//     dtFreeNavMesh(m_navMesh);
//     m_navMesh = 0;

//     if (m_tool)
//     {
//         m_tool->reset();
//         m_tool->init(this);
//     }
//     resetToolStates();
//     initToolStates(this);
// }


// bool Sample_SoloMesh::handleBuild()
// {
//     if (!m_geom || !m_geom->getMesh())
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Input mesh is not specified.");
//         return false;
//     }
    
//     cleanup();
    
//     // 模型包围盒
//     const float* bmin = m_geom->getNavMeshBoundsMin();
//     const float* bmax = m_geom->getNavMeshBoundsMax();
//     // 顶点数据
//     const float* verts = m_geom->getMesh()->getVerts();
//     const int nverts = m_geom->getMesh()->getVertCount();
//     // 三角形索引数据
//     const int* tris = m_geom->getMesh()->getTris();
//     const int ntris = m_geom->getMesh()->getTriCount();
    
//     //
//     // Step 1. Initialize build config.		// 第一步: 初始化 build 配置
//     //
    
//     // Init build configuration from GUI	// 从 GUI 获取配置参数
//     memset(&m_cfg, 0, sizeof(m_cfg));
//     m_cfg.cs = m_cellSize;
//     m_cfg.ch = m_cellHeight;
//     m_cfg.walkableSlopeAngle = m_agentMaxSlope;
//     m_cfg.walkableHeight = (int)ceilf(m_agentHeight / m_cfg.ch);
//     m_cfg.walkableClimb = (int)floorf(m_agentMaxClimb / m_cfg.ch);
//     m_cfg.walkableRadius = (int)ceilf(m_agentRadius / m_cfg.cs);
//     m_cfg.maxEdgeLen = (int)(m_edgeMaxLen / m_cellSize);
//     m_cfg.maxSimplificationError = m_edgeMaxError;
//     m_cfg.minRegionArea = (int)rcSqr(m_regionMinSize);		// Note: area = size*size
//     m_cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize);	// Note: area = size*size
//     m_cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
//     m_cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
//     m_cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;
    
//     // Set the area where the navigation will be build.
//     // Here the bounds of the input mesh are used, but the area could be specified by an user defined box, etc.
//     // 开辟构建空间, 根据包围盒计算网格大小获取NavMeshBoundsMin 和 NavMeshBoundsMax 参数
//     rcVcopy(m_cfg.bmin, bmin);
//     rcVcopy(m_cfg.bmax, bmax);
//     rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);	// 计算网格数量

//     // Reset build times gathering.			// 重置构建计时
//     m_ctx->resetTimers();

//     // Start the build process.				// 开始计时
//     m_ctx->startTimer(RC_TIMER_TOTAL);
    
//     m_ctx->log(RC_LOG_PROGRESS, "Building navigation:");
//     m_ctx->log(RC_LOG_PROGRESS, " - %d x %d cells", m_cfg.width, m_cfg.height);
//     m_ctx->log(RC_LOG_PROGRESS, " - %.1fK verts, %.1fK tris", nverts/1000.0f, ntris/1000.0f);
    
//     //
//     // Step 2. Rasterize input polygon soup.	// 第二步: 光栅化输入的多边形
//     //
    
//     // Allocate voxel heightfield where we rasterize our input data to.
//     m_solid = rcAllocHeightfield();	// 在 Heap 动态分配一块空间 用来存储光栅化之后的体素数据
//     if (!m_solid)
//     {
//         // 如果为 Null, 则 Heap 的空间不够分配 solid 了
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
//         return false;
//     }
//     // 分配二维网格空间, 每个格子是一个链表
//     if (!rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
//         return false;
//     }
    
//     // Allocate array that can hold triangle area types.
//     // If you have multiple meshes you need to process, allocate
//     // and array which can hold the max number of triangles you need to process.
//     // 分配 Heap 堆空间存储三角形数据
//     m_triareas = new unsigned char[ntris];
//     if (!m_triareas)
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", ntris);
//         return false;
//     }
    
//     // Find triangles which are walkable based on their slope and rasterize them. 
//     // If your input data is multiple meshes, you can transform them here, calculate
//     // the are type for each of the meshes and rasterize them. 
//     // 根据斜率找到可行走的三角形并对其进行光栅化。如果您的输入数据是多个网格，您可以在此处转换它们，计算每个网格的类型并将它们光栅化。
//     memset(m_triareas, 0, ntris*sizeof(unsigned char));	// 清空数据
//     // 根据倾斜度查找可行走的三角形, 如果三角形可行走 则将三角形ID标记为 RC_WALKABLE_AREA
//     rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, verts, nverts, tris, ntris, m_triareas);
//     // 光栅化三角形, 转换成体素
//     if (!rcRasterizeTriangles(m_ctx, verts, nverts, tris, m_triareas, ntris, *m_solid, m_cfg.walkableClimb))
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not rasterize triangles.");
//         return false;
//     }

//     // 是否保留中间结果。m_triareas中的数据已经存贮到了span中, m_triareas可以删除了
//     if (!m_keepInterResults)
//     {
//         delete [] m_triareas;
//         m_triareas = 0;
//     }
    
//     //
//     // Step 3. Filter walkables surfaces.	// 第三步: 过滤可走表面
//     //
    
//     // Once all geoemtry is rasterized, we do initial pass of filtering to
//     // remove unwanted overhangs caused by the conservative rasterization
//     // as well as filter spans where the character cannot possibly stand.
//     // 一旦完成所有几何面的光栅化, 我们移除掉因“保守光栅化”引起的无用的悬崖, 以及角色不可能站立的位置
//     if (m_filterLowHangingObstacles)    // 在walkableClimp范围内, 允许从低一级span过渡到当前span, 比如楼梯
//         rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
//     if (m_filterLedgeSpans)             // 过滤掉峭壁, 将峭壁两侧的span设置为不可走。判断高度差是否大于玩家攀爬高度
//         rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
//     if (m_filterWalkableLowHeightSpans) // 过滤掉上下间隔太小的span
//         rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);


//     //
//     // Step 4. Partition walkable surface to simple regions.	// 第四步: 把可行走表面划分为简单区域
//     //

//     // Compact the heightfield so that it is faster to handle from now on.
//     // This will result more cache coherent data as well as the neighbours
//     // between walkable cells will be calculated.
//     // 构建紧凑高度场。会有更多的缓存数据，此外可行走单元的邻接格子也会计算到。  
//     m_chf = rcAllocCompactHeightfield();
//     if (!m_chf)
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
//         return false;
//     }
//     // 从表示实心空间的heightfield构建表示开放空间的紧凑heightfield。
//     if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
//         return false;
//     }
    
//     if (!m_keepInterResults)	// 是否保留实心高度场的原数据
//     {
//         rcFreeHeightField(m_solid);
//         m_solid = 0;
//     }
        
//     // Erode the walkable area by agent radius.	// 通过 Agent 半径, 收紧可走区域
//     if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf))
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
//         return false;
//     }

//     // (Optional) Mark areas.	// 可选, 标记区域
//     const ConvexVolume* vols = m_geom->getConvexVolumes();
//     for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
//         rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);

    
//     // Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
//     // 划分高度场, 分割联通区域, 这样我们就可以用简单的算法去三角形化可走区域
//     // There are 3 partitioning methods, each with some pros and cons:  有 3 种分割区域的算法，每种方法都有一些优点和缺点：
//     // 1) Watershed partitioning    分水岭算法(默认的)
//     //   - the classic Recast partitioning
//     //   - creates the nicest tessellation
//     //   - usually slowest
//     //   - partitions the heightfield into nice regions without holes or overlaps
//     //   - the are some corner cases where this method creates produces holes and overlaps
//     //      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
//     //      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
//     //   * generally the best choice if you precompute the navmesh, use this if you have large open areas
//     //   * 适用于：需要预先计算navmesh ; 大型地图。  
//     // 2) Monotone partioning       单调 
//     //   - fastest
//     //   - partitions the heightfield into regions without holes and overlaps (guaranteed)
//     //   - creates long thin polygons, which sometimes causes paths with detours
//     //   * use this if you want fast navmesh generation
//     //   * 适用于：需要快速生成navemesh。  
//     // 3) Layer partitoining        按层次 
//     //   - quite fast
//     //   - partitions the heighfield into non-overlapping regions
//     //   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
//     //   - produces better triangles than monotone partitioning
//     //   - does not have the corner cases of watershed partitioning
//     //   - can be slow and create a bit ugly tessellation (still better than monotone)
//     //     if you have large open areas with small obstacles (not a problem if you use tiles)
//     //   * good choice to use for tiled navmesh with medium and small sized tiles
//     //   * 适用于：tile网格的中小型地图。  
    
//     if (m_partitionType == SAMPLE_PARTITION_WATERSHED)
//     {
//         // Prepare for region partitioning, by calculating distance field along the walkable surface.
//         if (!rcBuildDistanceField(m_ctx, *m_chf))
//         {
//             m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
//             return false;
//         }
        
//         // Partition the walkable surface into simple regions without holes.
//         if (!rcBuildRegions(m_ctx, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
//         {
//             m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
//             return false;
//         }
//     }
//     else if (m_partitionType == SAMPLE_PARTITION_MONOTONE)	// 单调分割
//     {
//         // Partition the walkable surface into simple regions without holes.
//         // Monotone partitioning does not need distancefield.
//         // 将可行走表面划分成简单地区, 无孔, 单调划分不需要距离范围
//         if (!rcBuildRegionsMonotone(m_ctx, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
//         {
//             m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
//             return false;
//         }
//     }
//     else // SAMPLE_PARTITION_LAYERS
//     {
//         // Partition the walkable surface into simple regions without holes.
//         if (!rcBuildLayerRegions(m_ctx, *m_chf, 0, m_cfg.minRegionArea))
//         {
//             m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
//             return false;
//         }
//     }
    
//     //
//     // Step 5. Trace and simplify region contours.  第五步: 跟踪并且简化区域轮廓
//     //
    
//     // Create contours.
//     m_cset = rcAllocContourSet();
//     if (!m_cset)
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
//         return false;
//     }
//     if (!rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
//         return false;
//     }
    
//     //
//     // Step 6. Build polygons mesh from contours.   第六步: 从轮廓生成凸多边形网格rcPolyMesh
//     //
    
//     // Build polygon navmesh from the contours.
//     m_pmesh = rcAllocPolyMesh();
//     if (!m_pmesh)
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
//         return false;
//     }
//     if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
//         return false;
//     }
    
//     //
//     // Step 7. Create detail mesh which allows to access approximate height on each polygon.
//     // 第七步: 创建细节网格, 生成高度细节 rcPolyMeshDetail
//     // 在这最后的阶段，凸多边形网格会被Delaunay三角化算法三角化，可以增加高度的细节。
//     //
    
//     m_dmesh = rcAllocPolyMeshDetail();
//     if (!m_dmesh)
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
//         return false;
//     }

//     if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
//     {
//         m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
//         return false;
//     }

//     if (!m_keepInterResults)
//     {
//         rcFreeCompactHeightfield(m_chf);
//         m_chf = 0;
//         rcFreeContourSet(m_cset);
//         m_cset = 0;
//     }

//     /**************************** 数据build完成 **************************************/

//     // At this point the navigation mesh data is ready, you can access it from m_pmesh.
//     // See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.
//     // 现在 Navmesh 数据就可以使用了, 你可以通过 m_pmesh 来使用数据。
//     // duDebugDrawPolyMesh 和 dtCreateNavMeshData 有使用样例。
    
//     //
//     // (Optional) Step 8. Create Detour data from Recast poly mesh.
//     //
    
//     // The GUI may allow more max points per polygon than Detour can handle.
//     // Only build the detour navmesh if we do not exceed the limit.
//     // GUI 允许每个多边形的最大点数可以比 Detour 能处理的更多，但只有在不超过限制(<=6)的情况下才能构建Detour导航网格
//     if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
//     {
//         unsigned char* navData = 0;
//         int navDataSize = 0;

//         // Update poly flags from areas.    设置多边形区域area的类型
//         for (int i = 0; i < m_pmesh->npolys; ++i)
//         {
//             if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
//                 m_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;
                
//             if (m_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
//                 m_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
//                 m_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
//             {
//                 m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
//             }
//             else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
//             {
//                 m_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
//             }
//             else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
//             {
//                 m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
//             }
//         }

//         // 填充参数，用于生成最终给Detour用的dtNavMesh
//         dtNavMeshCreateParams params;
//         memset(&params, 0, sizeof(params));
//         params.verts = m_pmesh->verts;
//         params.vertCount = m_pmesh->nverts;
//         params.polys = m_pmesh->polys;
//         params.polyAreas = m_pmesh->areas;
//         params.polyFlags = m_pmesh->flags;
//         params.polyCount = m_pmesh->npolys;
//         params.nvp = m_pmesh->nvp;
//         params.detailMeshes = m_dmesh->meshes;
//         params.detailVerts = m_dmesh->verts;
//         params.detailVertsCount = m_dmesh->nverts;
//         params.detailTris = m_dmesh->tris;
//         params.detailTriCount = m_dmesh->ntris;
//         params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
//         params.offMeshConRad = m_geom->getOffMeshConnectionRads();
//         params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
//         params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
//         params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
//         params.offMeshConUserID = m_geom->getOffMeshConnectionId();
//         params.offMeshConCount = m_geom->getOffMeshConnectionCount();
//         params.walkableHeight = m_agentHeight;
//         params.walkableRadius = m_agentRadius;
//         params.walkableClimb = m_agentMaxClimb;
//         rcVcopy(params.bmin, m_pmesh->bmin);
//         rcVcopy(params.bmax, m_pmesh->bmax);
//         params.cs = m_cfg.cs;
//         params.ch = m_cfg.ch;
//         params.buildBvTree = true;
        
//         if (!dtCreateNavMeshData(&params, &navData, &navDataSize))  // 用dtNavMeshCreateParams参数初始化navData
//         {
//             m_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
//             return false;
//         }
        
//         m_navMesh = dtAllocNavMesh();
//         if (!m_navMesh)
//         {
//             dtFree(navData);
//             m_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
//             return false;
//         }
        
//         dtStatus status;
        
//         status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);  // 根据navData初始化m_navMesh  
//         if (dtStatusFailed(status))
//         {
//             dtFree(navData);
//             m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
//             return false;
//         }
        
//         status = m_navQuery->init(m_navMesh, 2048); // 根据m_navMesh初始化查询类m_navQuery
//         if (dtStatusFailed(status))
//         {
//             m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
//             return false;
//         }
//     }

//     /************************ 生成数据成功 *****************************/
    
//     m_ctx->stopTimer(RC_TIMER_TOTAL);

//     // Show performance stats.  显示性能统计
//     duLogBuildTimes(*m_ctx, m_ctx->getAccumulatedTime(RC_TIMER_TOTAL));
//     m_ctx->log(RC_LOG_PROGRESS, ">> Polymesh: %d vertices  %d polygons", m_pmesh->nverts, m_pmesh->npolys);
    
//     m_totalBuildTimeMs = m_ctx->getAccumulatedTime(RC_TIMER_TOTAL)/1000.0f;
    
//     if (m_tool)
//         m_tool->init(this);
//     initToolStates(this);

//     return true;
// }
