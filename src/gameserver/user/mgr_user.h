#pragma once

#include "src/base/common/singleton.h"

#include <memory>
#include <map>

class MgrUser : public Singleton<MgrUser>
{
public:
    // MgrUser();
    // virtual ~MgrUser();

private:
    std::map<QWORD, std::shared_ptr<class User>> m_mapUser;
};
