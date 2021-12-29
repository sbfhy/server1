#include "c2g_scene.h"

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

    SRpcChannelMethodArgs* pMethodArgs = static_cast<SRpcChannelMethodArgs*>(args); // TODO args使用简化一下
    if (!pMethodArgs) return ;
    UserPtr userPtr = MgrUser::Instance().GetUser(pMethodArgs->accid);
    if (!userPtr) return;
    userPtr->CreatePlayerPawn();
}

void C2G_SceneService::C2G_MoveState(const ::CMD::C2G_MoveStateArgPtr& request,
                                     const ::CMD::EmptyResponsePtr& response,
                                     void* args)
{

}

