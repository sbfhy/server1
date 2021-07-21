#pragma once

#include "muduo/base/define/define_variable.h"

class MgrBase
{
public:
    virtual void Wake() {}
    virtual void Init() {}
    virtual void Tick(QWORD usec) {}
};
