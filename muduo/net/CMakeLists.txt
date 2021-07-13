include(CheckFunctionExists)                        # CheckFunctionExists模块，用于check_function_exists检查某个函数是否存在

check_function_exists(accept4 HAVE_ACCEPT4)         # accept4函数如果不存在，在编译SocketsOps.cc时添加-DNO_ACCEPT4参数
if(NOT HAVE_ACCEPT4)
  set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()

set(net_SRCS
  common/acceptor.cpp
  common/buffer.cpp
  common/channel.cpp
  common/connector.cpp
  common/eventloop.cpp
  common/eventloop_thread.cpp
  common/eventloop_thread_pool.cpp
  common/inet_address.cpp
  common/poller.cpp
  poller/default_poller.cpp
  poller/epoll_poller.cpp
  poller/poll_poller.cpp
  common/socket.cpp
  common/sockets_ops.cpp
  common/tcp_client.cpp
  common/tcp_connection.cpp
  common/tcp_server.cpp
  common/timer.cpp
  common/timer_queue.cpp
  )

add_library(muduo_net ${net_SRCS})
target_link_libraries(muduo_net muduo_base)

install(TARGETS muduo_net DESTINATION lib)

file(GLOB netCommonHeaders "common/*.h")
install(FILES ${netCommonHeaders} DESTINATION include/muduo/net/common)

set(netPollerHeaders
  poller/epoll_poller.h
  poller/poll_poller.h
  )
install(FILES ${netPollerHeaders} DESTINATION include/muduo/net/poller)

add_subdirectory(protobuf)
add_subdirectory(protorpc)


