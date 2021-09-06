file(GLOB_RECURSE SRC_FILES  "*.cpp" "*.cc" "*.c")                        # 获取所有子目录下匹配到的文件, 返回一个SRC_CXX_FILES 的list
add_library(src_base ${SRC_FILES})
target_link_libraries(src_base myproto)

add_subdirectory(muduo/base/tests)
