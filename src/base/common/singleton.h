#pragma once

#include "muduo/base/common/noncopyable.h"
#include "muduo/base/define/define_new.h"

template <typename T>
class Singleton : public muduo::noncopyable
{
protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    static T* PInstance()
    {
        if (m_pInstance == nullptr)
        {
            static CGarbo m_garbo;
            m_pInstance = NEW T();
        }
        return m_pInstance;
    }

    static T& Instance()
    {
        return *PInstance();
    }

    static void Destroy()
    {
        if (m_pInstance)
        {
            DELETE(m_pInstance);
            m_pInstance = nullptr;
        }
    }

    static bool IsValid()
    {
        return m_pInstance != nullptr;
    }

private:
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            DELETE(m_pInstance);
        }
    };

private:
    static T* m_pInstance;
};

template <typename T> T* Singleton<T>::m_pInstance = nullptr;
