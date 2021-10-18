
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "debug")                               # 设置默认的CMAKE_BUILD_TYPE
endif()

# option(MUDUO_BUILD_EXAMPLES "Build Muduo examples" ON)      # 自定义一个选项开关
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)                         # 打开编译命令执行日志

# 设置编译参数CMAKE_CXX_FLAGS
set(CXX_FLAGS_LIST 
    -g
    # -DVALGRIND
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall                                                     # 显示所有警告
    -Wextra                                                   # 打印额外(可能您并不想要)的警告信息  
    -Werror
    # -Wconversion                                            # 当隐式类型转换可能改变值时给出警告  
    -Wno-unused-parameter                                     # 有未使用的参数，不警告
    -Wno-unused-function                                      # 未使用的函数，不警告
    # -Wold-style-cast                                          # 程序使用C风格的类型转换时给出警告
    -Woverloaded-virtual                                      # 重载虚函数名时给出警告  
    -Wpointer-arith                                           # 当在算术表达式中使用函数指针时给出警告  
    -Wshadow                                                  # 当一个局部变量掩盖了另一个局部变量时给出警告  
    -Wwrite-strings                                           # 将字符串转换为‘char*’时给出警告
    -march=native
    # -MMD
    -std=c++17
    -rdynamic
)
if(CMAKE_BUILD_BITS EQUAL 32)        # 如果是32位
    list(APPEND CXX_FLAGS_LIST "-m32")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")                    # 如果是Clang编译器
    list(APPEND CXX_FLAGS_LIST "-Wno-null-dereference")                                                                                   
    list(APPEND CXX_FLAGS_LIST "-Wno-sign-conversion")                                                                                    
    list(APPEND CXX_FLAGS_LIST "-Wno-unused-local-typedef")                                                                               
    list(APPEND CXX_FLAGS_LIST "-Wthread-safety")                                                                                         
    list(REMOVE_ITEM CXX_FLAGS_LIST "-rdynamic")                                                                                          
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS_LIST}")   # CXX_FLAGS_LIST作为输入，用空格替代分号,将结果填充到变量CMAKE_CXX_FLAGS


set(CMAKE_CXX_FLAGS_DEBUG "-O0")                              # 设置Debug专用参数
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")                   # 设置Release专用参数

message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

