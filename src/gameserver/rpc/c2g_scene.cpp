#include "c2g_scene.h"

#include "service/g2c_scene.pb.h"
#include "muduo/base/common/logging.h"
#include "src/gameserver/user/user.h"
#include "define/define_service.h"
#include "src/gameserver/user/mgr_user.h"


using namespace RPC;

void C2G_SceneService::C2G_NotifyLoadedScene(const ::CMD::C2G_NotifyLoadedSceneArgPtr& request,
                                             const ::CMD::EmptyResponsePtr& response,
                                             void* args)
{
    {LDBG("M_NET") << request->ShortDebugString(); }

    SRpcChannelMethodArgs* pMethodArgs = static_cast<SRpcChannelMethodArgs*>(args);
    if (!pMethodArgs) return ;

    CMD::G2C_CreatePlayerPawnArgPtr G2C_CreatePlayerPawnArg = std::make_shared<CMD::G2C_CreatePlayerPawnArg>();
    G2C_CreatePlayerPawnArg->set_accid(pMethodArgs->accid);

    UserPtr userPtr = MgrUser::Instance().GetUser(pMethodArgs->accid);
    if (!userPtr) return;
    userPtr->Send(G2C_CreatePlayerPawnArg);
}

