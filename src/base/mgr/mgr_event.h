#pragma once 

#include "src/base/common/tvarlist.h"

#include <list>
#include <unordered_map>
#include <vector>
#include <functional>

class xEntry;

typedef std::function<void(xEntry* sender, const IVarList&)> EventBindCallback;
const QWORD DEFAULTKEYQWORD = 0;

struct MonitorData {                                                // 监听数据
    DWORD eType {0};                                                // 注册事件
    QWORD qwKey {0};                                                // key
};
typedef std::vector<MonitorData> TVecMonitorData;

struct EventData {  // 事件数据
    DWORD type {0};
    xEntry* sender {nullptr};
    QWORD qwKey {0};
    CVarList varList;
};
typedef std::list<EventData> TListEventData;
typedef std::unordered_map<void*, TVecMonitorData> TMapMonitor;     // 注册方信息map
typedef std::unordered_map<void*, EventBindCallback> TMapCallBack;  // callback map
typedef std::unordered_map<QWORD, TMapCallBack> TMapEventBind;      // 同类型事件


class MgrEvent
{
public:
    MgrEvent();
    virtual ~MgrEvent();

    virtual void Tick(QWORD curUsec);

    void SetEventSize(DWORD size) { m_vecEventData.resize(size); }

    /* ********************************************************/
    /**
     * @brief sendEvent 事件触发
     * @param type                 事件类型
     * @param sender               事件发送方
     * @param qwKey                定义关键字
     */
    /* ********************************************************/
    void SendEvent(DWORD type, xEntry* sender, QWORD qwKey = 0, const CVarList& varList = CVarList());
    
    /* ********************************************************/
    /**
     * @brief sendDelayEvent 事件触发(两帧触发回调)
     * @param type                 事件类型
     * @param sender               事件发送方
     * @param qwKey                定义关键字
     */
    /* ********************************************************/
    void SendDelayEvent(DWORD type, xEntry* sender, QWORD qwKey = 0, const CVarList& varList = CVarList());
    
    /* ********************************************************/
    /**
     * @brief addSceneEventBind 增加事件绑定
     * @param type          事件类型
     * @param monitor       监听方
     * @param callback      事件回调
     * @param key           关注的key值
     * @return bool         true 成功 false 失败
     */
    /* ********************************************************/
    bool AddEventBind(DWORD type, void* pMonitor, EventBindCallback callback, QWORD qwKey = 0);
    
    /* ********************************************************/
    /**
     * @brief delSceneEventBind 删除事件绑定
     * @param type          事件类型
     * @param monitor       监听方
     * @return bool         true 成功 false 失败
     */
    /* ********************************************************/
    bool DelEventBind(void* pmonitor, DWORD type = 0);

private:
    void eventCallBack(TMapEventBind& mapEventBind, xEntry* sender, QWORD qwKey, const IVarList& varList);
    bool isSameCallBack(EventBindCallback& left, EventBindCallback& right);
    bool addEventData(DWORD type, TMapEventBind& mapEventBind, void* pMonitor, EventBindCallback& callback, QWORD qwKey);
    bool delEventData(TMapEventBind& mapEventBind, void* pMonitor, QWORD qwKey);
    void forCallBack(TMapCallBack map, xEntry* sender, const IVarList& varList);
    TMapEventBind* getMapEventBind(DWORD type);


private:
    // 事件数组, <type, <key, <pMonitor*, callback>>>
    std::vector<TMapEventBind> m_vecEventData;
    // 延迟事件list(两帧触发回调), 
    TListEventData m_listEventData;
    // 监听方map(建立监听方索引是为了快速删除，无索引的情况需要遍历删除), <pMonitor*, vec[MonitorData]>
    TMapMonitor m_mapMonitor;
};

