#include "mgr_event_scene.h"

MgrEventScene::MgrEventScene()
{
    SetEventSize(static_cast<DWORD>(ES_EVENT_TYPE_MAX));
}

void MgrEventScene::Tick(QWORD curUsec)
{
    MgrEvent::Tick(curUsec);
}

void MgrEventScene::SendEvent(EEventTypeScene type, xEntry* sender, QWORD qwKey/* = 0*/, const CVarList& varList/* = CVarList()*/)
{
    MgrEvent::SendEvent(static_cast<DWORD>(type), sender, qwKey, varList);
}

void MgrEventScene::SendDelayEvent(EEventTypeScene type, xEntry* sender, QWORD qwKey/* = 0*/, const CVarList& varList/* = CVarList()*/)
{
    MgrEvent::SendDelayEvent(static_cast<DWORD>(type), sender, qwKey, varList);
}

bool MgrEventScene::AddEventBind(EEventTypeScene type, void* pMonitor, EventBindCallback callback, QWORD qwKey/* = 0*/)
{
    return MgrEvent::AddEventBind(static_cast<DWORD>(type), pMonitor, callback, qwKey);
}

bool MgrEventScene::DelEventBind(void* pMonitor, EEventTypeScene type/* = 0*/)
{
    return MgrEvent::DelEventBind(pMonitor, static_cast<DWORD>(type));
}
