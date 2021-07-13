# set_source_files_properties(${pbOutputPath}/sudoku.pb.cc PROPERTIES COMPILE_FLAGS "-Wno-conversion -Wno-shadow")
# include_directories(${PROJECT_BINARY_DIR})

# add_library(sudoku_proto ${pbOutputPath}/sudoku.pb.cc)
# target_link_libraries(sudoku_proto protobuf pthread)

add_executable(protobuf_rpc_sudoku_client client.cc)
# set_target_properties(protobuf_rpc_sudoku_client PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
target_link_libraries(protobuf_rpc_sudoku_client myproto muduo_protorpc)

add_executable(protobuf_rpc_sudoku_server server.cc)
# set_target_properties(protobuf_rpc_sudoku_server PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
target_link_libraries(protobuf_rpc_sudoku_server myproto muduo_protorpc)

add_executable(protobuf_rpc_sudoku_server2 server2.cc)
target_link_libraries(protobuf_rpc_sudoku_server2 myproto muduo_protorpc)

# add_custom_target(protobuf_rpc_all
#                   DEPENDS
#                         muduo_protorpc
#                         protobuf_rpc_balancer
#                         protobuf_rpc_balancer_raw
#                         protobuf_rpc_echo_client
#                         protobuf_rpc_echo_server
#                         protobuf_rpc_resolver_client
#                         protobuf_rpc_resolver_server
#                         protobuf_rpc_sudoku_client
#                         protobuf_rpc_sudoku_server
#                         )
