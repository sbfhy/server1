#pragma once

#include "src/gameserver/scene/assets.h"
#include "src/gameserver/entity/define_entity.h"

#include <set>

namespace google {
namespace protobuf {

class Message;           // message.h
typedef ::std::shared_ptr<Message> MessagePtr;

} // namespace protobuf
} // namespace google


class Scene
{
public:
    virtual ~Scene();

private:
    Assets m_assets;


    // 场景广播
public:
    void Send(const ::google::protobuf::MessagePtr& request);


    // entity相关
public:
    void AddEntity(EntityDynamicPtr);
    void DelEntity(EntityDynamicPtr);
private:
    void G2C_CreatePlayerPawn(EntityDynamicPtr);
    std::set<EntityDynamicPtr> m_setEntityDynamic;
};
