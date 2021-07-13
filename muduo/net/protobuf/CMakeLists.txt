add_library(muduo_protobuf_codec ProtobufCodecLite.cc)
set_target_properties(muduo_protobuf_codec PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
target_link_libraries(muduo_protobuf_codec muduo_net ${Protobuf_LIBRARY} pthread z)
install(TARGETS muduo_protobuf_codec DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/muduo/net/protobuf)

