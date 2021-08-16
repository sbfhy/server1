# add_library(muduo_protorpc_wire RpcCodec.cc)
# set_target_properties(muduo_protorpc_wire PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
# target_link_libraries(muduo_protorpc_wire myproto)

# add_library(muduo_protorpc RpcChannel.cc RpcServer.cc RpcService.cc)
# target_link_libraries(muduo_protorpc muduo_protorpc_wire muduo_protobuf_codec muduo_net muduo_base ${Protobuf_LIBRARY} myproto rt z)

# if(TCMALLOC_INCLUDE_DIR AND TCMALLOC_LIBRARY)
#   target_link_libraries(muduo_protorpc tcmalloc_and_profiler)
# endif()

# install(TARGETS muduo_protorpc DESTINATION lib)

# set(HEADERS
#   RpcChannel.h
#   RpcServer.h
#   RpcService.h
#   service.h
#   )
# install(FILES ${HEADERS} DESTINATION include/muduo/net/protorpc)


