#pragma once

#include "define/define_variable.h"
#include "src/gameserver/entity/define_entity.h"
#include "src/gameserver/scene/define_scene.h"
#include "src/gameserver/user/define_user.h"

#include <memory>

namespace google {
namespace protobuf {

class Message;           // message.h
typedef ::std::shared_ptr<Message> MessagePtr;

} // namespace protobuf
} // namespace google

class User : public std::enable_shared_from_this<User>
{
public:
    User(QWORD accid) : m_accid(accid) {}
    virtual ~User();
private:
    UserWPtr weakFromThis() { return weak_from_this(); }


    // 账号消息相关
public:
    void Send(const ::google::protobuf::MessagePtr& request);
    QWORD GetAccid() const { return m_accid; }
private:
    QWORD m_accid {0};


    // 主控角色相关
public:
    EntityDynamicPtr GetPlayerPawn() const { return m_playerPawn; }
    void SetPlayerPawn(EntityDynamicPtr pPawn) { m_playerPawn = pPawn; }
    void GotoMap(ScenePtr);
    void CreatePlayerPawn();
    ScenePtr GetScene() { return m_wScene.lock(); }
    void SetScene(ScenePtr pScene) { m_wScene = pScene; }
private:
    void G2C_EnterScene();
    SceneWPtr m_wScene;
    EntityDynamicPtr m_playerPawn;
};

typedef std::shared_ptr<User> UserPtr;

