#pragma once

#include "service/g2c_scene.pb.h"
#include "src/base/mgr/create_dynamic.h"

namespace RPC {

class G2C_SceneService_Stub : public CreateDynamicEmpty<G2C_SceneService_Stub>
                            , public CMD::G2C_SceneService_Stub
{
public:
    G2C_SceneService_Stub();

    virtual void G2C_EnterScene_DoneCb(const ::CMD::G2C_EnterSceneArgPtr &request,
                                       const ::CMD::EmptyResponsePtr &response) {}

    virtual void G2C_EnterScene_TimeOut(const ::CMD::G2C_EnterSceneArgPtr &request) {}
};

}   // namespace RPC
