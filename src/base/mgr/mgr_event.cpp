#include "src/base/mgr/mgr_event.h"
#include "muduo/base/common/logging.h"

MgrEvent::MgrEvent()
{
}

MgrEvent::~MgrEvent()
{
}

void MgrEvent::Tick(QWORD curUsec)
{
    for (auto& l : m_listEventData)
    {
        SendEvent(l.type, l.sender, l.qwKey, l.varList);
    }
    m_listEventData.clear();
}

void MgrEvent::SendEvent(DWORD type, xEntry* sender, QWORD qwKey/* = 0*/, const CVarList& varList/* = CVarList()*/)
{
    TMapEventBind* mapEventBind = getMapEventBind(type);
    if (nullptr == mapEventBind)
        return;
    eventCallBack(*mapEventBind, sender, qwKey, varList);
}

void MgrEvent::SendDelayEvent(DWORD type, xEntry* sender, QWORD qwKey/* = 0*/, const CVarList& varList/* = CVarList()*/)
{
    TMapEventBind* mapEventBind = getMapEventBind(type);
    if (nullptr == mapEventBind)
        return;

    EventData event;
    event.type = type;
    event.sender = sender;
    event.qwKey = qwKey;
    event.varList = varList;

    m_listEventData.push_back(std::move(event));
}

bool MgrEvent::AddEventBind(DWORD type, void* pMonitor, EventBindCallback callback, QWORD qwKey/* = 0*/)
{
    TMapEventBind* mapEventBind = getMapEventBind(type);
    if (nullptr == mapEventBind)
        return false;
    return addEventData(type, *mapEventBind, pMonitor, callback, qwKey);
}

bool MgrEvent::DelEventBind(void* pMonitor, DWORD type/* = 0*/)
{
    if (m_mapMonitor.find(pMonitor) == m_mapMonitor.end())
        return true;

    TVecMonitorData& vecMonitor = m_mapMonitor[pMonitor];
    for (auto iter = vecMonitor.begin(); iter != vecMonitor.end();)
    {
        if (type > 0 && iter->eType != type)
        {
            // 指定删除类型
            ++iter;
            continue;
        }

        TMapEventBind* mapEventBind = getMapEventBind(iter->eType);
        if (nullptr == mapEventBind)
        {
            iter = vecMonitor.erase(iter);
            continue;
        }

        delEventData(*mapEventBind, pMonitor, iter->qwKey);
        iter = vecMonitor.erase(iter);
    }

    if (vecMonitor.size() == 0)
        m_mapMonitor.erase(pMonitor);

    return true;
}

void MgrEvent::eventCallBack(TMapEventBind& eventData, xEntry* sender, QWORD qwKey, const IVarList& varList)
{
    if (nullptr == sender)
        return;

    /* ********************************************************/
    /**
     * 生成key规则
     * "0"   默认 匹配任意npc
     * 自定义key 如npcid,guid
    */
    /* ********************************************************/

    // 自定义key
    if (qwKey > 0)
    {
        if (eventData.find(qwKey) != eventData.end())
        {
            forCallBack(eventData[qwKey], sender, varList);
        }
    }
    else
    {
        // 默认
        if (eventData.find(DEFAULTKEYQWORD) != eventData.end())
        {
            forCallBack(eventData[DEFAULTKEYQWORD], sender, varList);
        }
    }
}

bool MgrEvent::addEventData(DWORD type, TMapEventBind& mapEventBind, void* pMonitor, EventBindCallback& callback, QWORD qwKey)
{
    TMapCallBack& mapCallBack = mapEventBind[qwKey];
    if (mapCallBack.find(pMonitor) != mapCallBack.end())
        return false;

    mapCallBack[pMonitor] = callback;

    // 建立监听方索引
    {
        MonitorData monitorData;
        monitorData.eType = type;
        monitorData.qwKey = qwKey;

        if (m_mapMonitor.find(pMonitor) == m_mapMonitor.end())
            m_mapMonitor[pMonitor] = TVecMonitorData();
        TVecMonitorData& vecMonitor = m_mapMonitor[pMonitor];
        vecMonitor.push_back(monitorData);
    }

    return true;
}

bool MgrEvent::delEventData(TMapEventBind& mapEventBind, void* pMonitor, QWORD qwKey)
{
    if (mapEventBind.find(qwKey) == mapEventBind.end())
        return false;

    TMapCallBack& mapCallBack = mapEventBind[qwKey];
    auto iter = mapCallBack.find(pMonitor);
    if (iter == mapCallBack.end())
        return false;

    mapCallBack.erase(iter);
    return true;
}

bool MgrEvent::isSameCallBack(EventBindCallback& left, EventBindCallback& right)
{
    if (left.target<void(*)(QWORD)>() == right.target<void(*)(QWORD)>())
        return true;
    return false;
}

TMapEventBind* MgrEvent::getMapEventBind(DWORD type)
{
    if (type >= m_vecEventData.size())
    {
        return nullptr;
    }

    return &m_vecEventData[type];
}

void MgrEvent::forCallBack(TMapCallBack map, xEntry* sender, const IVarList& varList)
{
    for (const auto iter : map)
    {
        try
        {
            iter.second(sender, varList);
        }
        catch (std::bad_function_call& e)
        {
            LOG_ERROR << "[事件系统] 未设置bind函数";
        }
    }
};
