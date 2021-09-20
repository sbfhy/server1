#pragma once

#include "service/c2g_scene.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {
    
class C2G_SceneEntryService : public CreateDynamicEmpty<C2G_SceneEntryService>
                            , public CMD::C2G_SceneEntryService
{
public:
    C2G_SceneEntryService() {}
    
    virtual void C2G_CreateEntry(const ::CMD::C2G_CreateEntryArgPtr& request,
                                 const ::CMD::C2G_CreateEntryResPtr& response) override;

};

}   // namespace RPC
