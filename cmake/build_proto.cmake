# 设置proto
set(protoDir ${CMAKE_SOURCE_DIR}/myproto/proto)
set(pbOutputDir ${CMAKE_SOURCE_DIR}/myproto/include)

# 编译proto插件 =====================================
set(protoPluginDir ${CMAKE_SOURCE_DIR}/myproto/proto/protoc_gen_rpc)
message("protoPluginDir: ${protoPluginDir}")
add_executable(protoc_gen_rpc ${protoPluginDir}/gen.cc ${protoPluginDir}/cpp_service.cc)
target_link_libraries(protoc_gen_rpc ${Protobuf_PROTOC_LIBRARY} ${Protobuf_LIBRARY} pthread)
# add_custom_command(                           # FIXME: fix this command
#     TARGET protoc_gen_rpc PRE_BUILD
#     COMMENT "Generating protoc_gen_rpc    ----------------"
#     # COMMAND sed  ARGS -i "/option cc_generic_services/d" ${protoDir}/rpcservice.proto  # 去掉cc_generic_services这个设置
#     # DEPENDS ${protoPluginDir}/cpp_service.cc
#     VERBATIM 
# )
install(TARGETS protoc_gen_rpc DESTINATION bin)


# 编译pb  ===========================================
file(GLOB_RECURSE protoFiles ${protoDir}/*.proto)       # 收集所有proto文件

set(pbOutputSrcFiles "")
set(pbOutputHeadFiles "")

foreach(protoFile ${protoFiles})
    string(REPLACE ${protoDir} "" suffix ${protoFile})
    set(outputPath ${pbOutputDir}${suffix})

    string(REPLACE ".proto" ".pb.h" pbHeadFile ${outputPath})
    string(REPLACE ".proto" ".pb.cc" pbSrcFile ${outputPath})

    list(APPEND pbOutputHeadFiles ${pbHeadFile})
    list(APPEND pbOutputSrcFiles ${pbSrcFile})

    get_filename_component(outputDir ${outputPath} DIRECTORY)

    # add_custom_command(                             # 不使用插件
    #     OUTPUT ${pbSrcFile} ${pbHeadFile}
    #     COMMAND ${Protobuf_PROTOC_EXECUTABLE}   --cpp_out=${pbOutputDir}  ${protoFile} -I${protoDir} 
    #     DEPENDS ${protoFile}
    #     COMMENT "Generate protobuf, pbHeadFile:${pbHeadFile}, pbSrcFile:${pbSrcFile}" 
    #     VERBATIM 
    # )

    add_custom_command(                               # 使用protoc-gen-rpc插件
      OUTPUT ${pbSrcFile} ${pbHeadFile}
      COMMAND ${Protobuf_PROTOC_EXECUTABLE} --plugin=protoc-gen-rpc=${EXECUTABLE_OUTPUT_PATH}/protoc_gen_rpc --cpp_out=${pbOutputDir} --rpc_out=${pbOutputDir} ${protoFile}  -I${protoDir} 
    #   COMMAND sed  ARGS -i "s/#include .rpc.pb.h./#include \"rpc.pb.h\"/" ${pbHeadFile}
    #   COMMAND sed  ARGS -i  "/protoc_insertion_point.includes./i#include <memory>" ${pbHeadFile}
      # COMMAND sed  ARGS -i  "/protoc_insertion_point.namespace_scope./itypedef ::std::shared_ptr<Empty> EmptyPtr;" ${pbHeadFile}
      COMMENT "Generate protobuf pb files: ${pbSrcFile} ${pbHeadFile} " 
      MAIN_DEPENDENCY ${protoFile}
      DEPENDS protoc_gen_rpc 
      VERBATIM 
      )
endforeach()


# 设置某些源文件编译属性
# set_source_files_properties(${pbOutputDir}/rpc.pb.cc PROPERTIES GENERATED yes)
# set_source_files_properties(${pbOutputDir}/rpc.pb.cc ${pbOutputDir}/rpcservice.pb.cc PROPERTIES COMPILE_FLAGS "-Wno-conversion")
# set_source_files_properties(${pbOutputDir}/sudoku.pb.cc PROPERTIES COMPILE_FLAGS "-Wno-conversion -Wno-shadow")


# 包含编译出来的pb头文件 ============================
include_directories(${pbOutputDir})
install(FILES ${pbOutputHeadFiles} DESTINATION include)


# 打包成库  =======================================
add_library(myproto ${pbOutputSrcFiles})
target_link_libraries(myproto ${Protobuf_PROTOC_LIBRARY} ${Protobuf_LIBRARY} pthread -lz)
add_dependencies(myproto protoc_gen_rpc)
install(TARGETS myproto DESTINATION lib)
