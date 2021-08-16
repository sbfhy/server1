add_executable(simple_echo echo/echo_server.cpp echo/main.cpp)
target_link_libraries(simple_echo src_base)

add_executable(simple_helloworld helloworld/main.cpp)
#target_link_libraries(simple_helloworld)


