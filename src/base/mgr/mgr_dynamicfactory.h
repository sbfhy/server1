// 参考 [C++实现反射(根据类名动态创建对象](https://blog.csdn.net/heyuhang112/article/details/51729435)  

#ifndef __DYNAMIC_FACTORY_H__
#define __DYNAMIC_FACTORY_H__

#ifdef __GNUC__
#include <cxxabi.h>
#endif // __GNUC__

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <typeinfo>

#include "src/base/common/singleton.h"

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

class DynamicObject;


// 动态对象创建工厂
class MgrDynamicFactory : public Singleton<MgrDynamicFactory>
{
public:
    typedef DynamicObject* (*CreateFunction)();
    typedef muduo::net::Service* PService;
    typedef PService (*CreateServiceFunc)();

    // 解析类型名称（转换为 A::B::C 的形式）
    // GCC 的type_info::name()输出的名称很猥琐，这里只做简单的解析，只支持自定义的结构体（非模板），类（非模板）、枚举、联合
    static std::string ReadTypeName(const char *name);

    bool Regist(const char *name, CreateFunction func);
    DynamicObject *Create(const std::string &type_name);

    template <typename T>
    T *Create(const std::string &type_name);


    // service
    bool RegistService(const char *name, CreateServiceFunc func);
    PService CreateService(const std::string &type_name);

private:
    std::map<std::string, CreateFunction> _create_function_map;
    std::map<std::string, CreateServiceFunc> m_mapCreateServiceFunc;
};

#endif  // __DYNAMIC_FACTORY_H__
