#pragma once

#include "dynamic_object.h"
#include "src/base/mgr/mgr_dynamicfactory.h"

// 动态对象创建器
// 注意：使用gcc，CreateDynamic的派生类一定要显式声明构造函数，否则不会执行注册代码
template <typename T>
class CreateDynamic : public DynamicObject
{
public:
    static DynamicObject *CreateObject()
    {
        return new T();
    }

    struct Registor
    {
        Registor()
        {
            if (!MgrDynamicFactory::Instance().Regist(typeid(T).name(), CreateObject))
            {
                assert(false);
            }
        }

        inline void do_nothing() const {}
    };

    static Registor s_registor;

public:
    CreateDynamic()
    {
        s_registor.do_nothing();
    }

    virtual ~CreateDynamic()
    {
        s_registor.do_nothing();
    }
};

template <typename T>
typename CreateDynamic<T>::Registor CreateDynamic<T>::s_registor;


// ===================================================================

template <typename T>
class CreateDynamicEmpty
{
public:
    static DynamicObject *CreateObject()
    {
        return new T();
    }

    struct Registor
    {
        Registor()
        {
            if (!MgrDynamicFactory::Instance().Regist(typeid(T).name(), CreateObject))
            {
                assert(false);
            }
        }

        inline void do_nothing() const {}
    };

    static Registor s_registor;

public:
    CreateDynamicEmpty()
    {
        s_registor.do_nothing();
    }

    virtual ~CreateDynamicEmpty()
    {
        s_registor.do_nothing();
    }
};

template <typename T>
typename CreateDynamicEmpty<T>::Registor CreateDynamicEmpty<T>::s_registor;

