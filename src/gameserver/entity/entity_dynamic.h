#pragma once

#include "define/define_variable.h"
#include "src/gameserver/entity/entity.h"
#include "src/gameserver/scene/define_scene.h"
#include "src/gameserver/user/define_user.h"

namespace google {
namespace protobuf {

class Message;           // message.h
typedef ::std::shared_ptr<Message> MessagePtr;

} // namespace protobuf
} // namespace google


class EntityDynamic : public Entity
                    , public std::enable_shared_from_this<EntityDynamic>
{
public:
    ~EntityDynamic();

public:
    QWORD GetGuid() const { return GetAccid(); }    // FIXME guid暂时用accid
private:
    void setGuid(QWORD guid) { m_guid = guid; } 
    QWORD m_guid {0};


    // Scene场景相关
public: 
    bool EnterScene(ScenePtr);
    void LeaveScene();
    ScenePtr GetScene() const { return m_wScene.lock(); }
private:
    void setScene(ScenePtr pScene) { m_wScene = pScene; }
    void resetScene() { m_wScene.reset(); }
    void onEnterScene();
    void onLeaveScene();
    SceneWPtr m_wScene;


    // User用户相关
public:
    UserPtr GetUser() const { return m_wUser.lock(); }
    void SetUser(UserPtr user); 
    QWORD GetAccid() const { return m_accid; }
    void Send(const ::google::protobuf::MessagePtr& request);
private:
    UserWPtr m_wUser;
    QWORD m_accid {0};  // 记录一下，避免频繁转换weak_ptr
};

