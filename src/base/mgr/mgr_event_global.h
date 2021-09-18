#pragma once

#include "mgr_event.h"
#include "src/base/common/singleton.h"
#include "message/common/event_enum.pb.h"

#include <list>

using namespace ENUM;

/**
 * 全局事件管理器类(适用全局事件，如登录，登出，场景创建，场景销毁等等)
 */
class MgrEventGlobal : public MgrEvent
                     , public Singleton<MgrEventGlobal>
{
  friend class Singleton<MgrEventGlobal>;
  public:
    MgrEventGlobal();

    virtual void Tick(QWORD curUsec) override;

  public:
    /* ********************************************************/
    /**
     * @brief sendEvent 事件触发
     * @param type                 事件类型
     * @param sender               事件发送方
     * @param qwKey                关键字
     */
    /* ********************************************************/
    void SendEvent(EEventTypeGlobal type, xEntry* sender, QWORD qwKey = 0, const CVarList& varList = CVarList());
    
    /* ********************************************************/
    /**
     * @brief sendDelayEvent 事件触发(两帧触发回调)
     * @param type                 事件类型
     * @param sender               事件发送方
     * @param qwKey                关键字
     */
    /* ********************************************************/
    void SendDelayEvent(EEventTypeGlobal type, xEntry* sender, QWORD qwKey = 0, const CVarList& varList = CVarList());
    
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
    bool AddEventBind(EEventTypeGlobal type, void* pMonitor, EventBindCallback callback, QWORD qwKey = 0);
    
    /* ********************************************************/
    /**
     * @brief delSceneEventBind 删除事件绑定
     * @param type          事件类型
     * @param monitor       监听方
     * @return bool         true 成功 false 失败
     */
    /* ********************************************************/
    bool DelEventBind(void* pmonitor, EEventTypeGlobal type = EG_EVENT_TYPE_MIN);
};

#define ADD_EVENT_GLOBAL_3(a, b, c)                 MgrEventGlobal::Instance().AddEventBind(a, b, c)
#define ADD_EVENT_GLOBAL_4(a, b, c, d)              MgrEventGlobal::Instance().AddEventBind(a, b, c, d)
#define SEND_EVENT_GLOBAL_3(a, b, c)                MgrEventGlobal::Instance().SendEvent(a, b, c)
#define SEND_EVENT_GLOBAL_4(a, b, c, d)             MgrEventGlobal::Instance().SendEvent(a, b, c, d)
