#pragma once

#include "src/base/mgr/dynamic_object.h"
#include "src/base/mgr/mgr_dynamicfactory.h"

#include "muduo/base/common/logging.h"

namespace google {
namespace protobuf {

class Service;

}   // namespace protobuf
}   // namespace google

namespace muduo {
namespace net {

class Service;

}   // namespace net
}   // namespace muduo 


// 注意：使用gcc，CreateDynamic的派生类一定要显式声明构造函数，否则不会执行注册代码
template <typename T>
class CreateService 
{
public:
    static muduo::net::Service* CreateObject()
    {
        return new T();
    }

    struct Registor
    {
        Registor()
        {
            // {LDBG("M_NET") << this << " CreateService 注册, " << typeid(T).name() << ", " << &v;}
            if (!MgrDynamicFactory::Instance().RegistService(typeid(T).name(), CreateObject))
            {
                assert(false);
            }
        }

        inline void do_nothing() const {}

        int v = 0;
    };

    static int s_a;
    static Registor s_registor;

public:
    CreateService()
    {
        s_registor.do_nothing();
    }
    
    virtual ~CreateService()
    {
        s_registor.do_nothing();
    }
};

template <typename T>
typename CreateService<T>::Registor CreateService<T>::s_registor;

template <typename T>
int CreateService<T>::s_a = 0;
