#pragma once

#include "service/c2g_scene.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {
    
class C2G_SceneService : public CreateDynamicEmpty<C2G_SceneService>
                       , public CMD::C2G_SceneService
{
public:
    C2G_SceneService() {}
    
    virtual void C2G_NotifyLoadedScene(const ::CMD::C2G_NotifyLoadedSceneArgPtr& request,
                                       const ::CMD::EmptyResponsePtr& response,
                                       void* args) override;

    virtual void C2G_MoveState(const ::CMD::C2G_MoveStateArgPtr& request,
                               const ::CMD::EmptyResponsePtr& response,
                               void* args) override;
};

}   // namespace RPC
