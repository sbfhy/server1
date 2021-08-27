#include "src/base/mgr/mgr_dynamicfactory.h"

#include "dynamic_object.h"
#include "muduo/base/common/logging.h"

// 解析类型名称（转换为 A::B::C 的形式）
// GCC 的type_info::name()输出的名称很猥琐，这里只做简单的解析，只支持自定义的结构体（非模板），类（非模板）、枚举、联合
std::string MgrDynamicFactory::ReadTypeName(const char *name)
{
#ifndef __GNUC__
    const char *p = strstr(name, " ");
    if (p)
    {
        size_t prev_len = (size_t)(p - name);
        if (memcmp(name, "class", prev_len) == 0 ||
            memcmp(name, "struct", prev_len) == 0 ||
            memcmp(name, "enum", prev_len) == 0 ||
            memcmp(name, "union", prev_len) == 0)
        {
            p += 1;
            return std::string(p);
        }
    }

    return std::string(name);
#else
    char *real_name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
    std::string real_name_string(real_name);
    free(real_name);
    return real_name_string;
#endif // __GNUC__
}

bool MgrDynamicFactory::Regist(const char *name, CreateFunction func)
{
    if (!func)
    {
        return false;
    }
    std::string type_name = ReadTypeName(name);
    return _create_function_map.insert(std::make_pair(type_name, func)).second;
}

DynamicObject *MgrDynamicFactory::Create(const std::string &type_name)
{
    if (type_name.empty())
    {
        return NULL;
    }

    std::map<std::string, CreateFunction>::iterator it = _create_function_map.find(type_name);
    if (it == _create_function_map.end())
    {
        return NULL;
    }

    return it->second();
}

template <typename T>
T *MgrDynamicFactory::Create(const std::string &type_name)
{
    DynamicObject *obj = Create(type_name);
    if (!obj)
    {
        return NULL;
    }
    T *real_obj = dynamic_cast<T *>(obj);
    if (!real_obj)
    {
        delete obj;
        return NULL;
    }
    return real_obj;
}
