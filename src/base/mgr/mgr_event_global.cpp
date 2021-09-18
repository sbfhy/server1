#include "mgr_event_global.h"

using namespace ENUM;

MgrEventGlobal::MgrEventGlobal()
{
    SetEventSize(static_cast<DWORD>(EG_EVENT_TYPE_MAX));
}

void MgrEventGlobal::Tick(QWORD curUsec)
{
    MgrEvent::Tick(curUsec);
}

void MgrEventGlobal::SendEvent(EEventTypeGlobal type, xEntry* sender, QWORD qwKey/* = 0*/, const CVarList& varList/* = CVarList()*/)
{
    MgrEvent::SendEvent(static_cast<DWORD>(type), sender, qwKey, varList);
}

void MgrEventGlobal::SendDelayEvent(EEventTypeGlobal type, xEntry* sender, QWORD qwKey/* = 0*/, const CVarList& varList/* = CVarList()*/)
{
    MgrEvent::SendDelayEvent(static_cast<DWORD>(type), sender, qwKey, varList);
}

bool MgrEventGlobal::AddEventBind(EEventTypeGlobal type, void* pMonitor, EventBindCallback callback, QWORD qwKey/* = 0*/)
{
    return MgrEvent::AddEventBind(static_cast<DWORD>(type), pMonitor, callback, qwKey);
}

bool MgrEventGlobal::DelEventBind(void* pMonitor, EEventTypeGlobal type/* = 0*/)
{
    return MgrEvent::DelEventBind(pMonitor, static_cast<DWORD>(type));
}
