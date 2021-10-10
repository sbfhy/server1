set(TARGET navmesh)

file(GLOB SOURCES source/*.cpp fastlz/fastlz.c)
add_library(${TARGET} 
            ${SOURCES}
            )

add_dependencies(${TARGET} 
                 recastnavigation
                 srcbase
                 )

target_include_directories( ${TARGET} PUBLIC 
                            $<BUILD_INTERFACE:fastlz 
                                              ${CMAKE_INSTALL_PREFIX}/include/recastnavigation>
                            )
include_directories(include)

target_link_libraries(${TARGET} 
                      DetourCrowd 
                      DetourTileCache 
                      Detour 
                      Recast
                      )


