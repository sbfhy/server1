#pragma once

#include "define/define_variable.h"

class MgrBase
{
public:
    virtual void Wake() {}
    virtual bool Init() { return true; }
    virtual void Tick(QWORD curUsec) {}
};
