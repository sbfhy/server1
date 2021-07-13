cmake_minimum_required(VERSION 3.0)

project(rpcserver1)

cmake_policy(SET CMP0015 NEW) # 设置cmake方案 CMP0015 https://cmake.org/cmake/help/v3.0/policy/CMP0015.html

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)  # 设置cmake模块路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)         # 设置生成执行文件的目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)            # 设置生成库文件的目录

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)                # 转成大写，输出到变量BUILD_TYPE
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})


# ==============================================
include(set_cmake_flag)                                       # 设置cmake各种flag
include(find_package)                                         # 查找依赖库


# 添加包含目录 =================================
include_directories(${PROJECT_SOURCE_DIR})
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()
if(PROTOBUF_FOUND)
  message(STATUS "include protobuf directories : ${PROTOBUF_INCLUDE_DIR}")
  include_directories(${PROTOBUF_INCLUDE_DIR})                # 包含protobuf头文件
endif()


# 编译 ==========================================
include(build_proto)                                          # 编译pb


# 添加需要构建的子目录 ==========================
add_subdirectory(muduo/base)                                  
add_subdirectory(muduo/net)
add_subdirectory(examples)
add_subdirectory(src)
