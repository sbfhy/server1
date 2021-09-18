#pragma once

#include <algorithm>
#include <stdint.h>


typedef unsigned int DWORD;
typedef int SDWORD;
typedef char CHAR;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef short SWORD;
typedef uintptr_t UINTPTR;
typedef intptr_t INTPTR;
typedef uint64_t QWORD;
typedef int64_t SQWORD;
typedef float FLOAT;
typedef double DOUBLE;


enum VARIANT_TYPE           // value定义
{
    VTYPE_UNKNOWN = 0,      // 未知
    VTYPE_BOOL,             // 布尔
    VTYPE_DWORD,            // 32位无符号整数
    VTYPE_QWORD,            // 64位无符号整数
    VTYPE_FLOAT,            // 单精度浮点数
    VTYPE_DOUBLE,           // 双精度浮点数
    VTYPE_STRING,           // 字符串
    VTYPE_POINTER,          // 指针
    VTYPE_MAX,
};
