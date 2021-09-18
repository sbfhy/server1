#pragma once

#include "mgr_event.h"
#include "message/common/event_enum.pb.h"

#include <list>

using namespace ENUM;


class MgrEventScene : public MgrEvent
{
  public:
    MgrEventScene();

    virtual void Tick(QWORD curUsec) override;

  public:
    /* ********************************************************/
    /**
     * @brief sendEvent 事件触发
     * @param type                 事件类型
     * @param sender               事件发送方
     * @param qwKey                定义关键字
     */
    /* ********************************************************/
    void SendEvent(EEventTypeScene type, xEntry* sender, QWORD qwKey = 0, const CVarList& varList = CVarList());
    
    /* ********************************************************/
    /**
     * @brief sendDelayEvent 事件触发(两帧触发回调)
     * @param type                 事件类型
     * @param sender               事件发送方
     * @param qwKey                定义关键字
     */
    /* ********************************************************/
    void SendDelayEvent(EEventTypeScene type, xEntry* sender, QWORD qwKey = 0, const CVarList& varList = CVarList());
    
    /* ********************************************************/
    /**
     * @brief addSceneEventBind 增加事件绑定
     * @param type          事件类型
     * @param monitor       监听方
     * @param callback      事件回调void 
     * @param key           关注的key值
     * @return bool         true 成功 false 失败
     */
    /* ********************************************************/
    bool AddEventBind(EEventTypeScene type, void* pMonitor, EventBindCallback callback, QWORD qwKey = 0);
    
    /* ********************************************************/
    /**
     * @brief delSceneEventBind 删除事件绑定
     * @param type          事件类型
     * @param monitor       监听方
     * @return bool         true 成功 false 失败
     */
    /* ********************************************************/
    bool DelEventBind(void* pmonitor, EEventTypeScene type = ES_EVENT_TYPE_MIN);
};
