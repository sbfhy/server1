set(TARGET navmesh)

file(GLOB SOURCES Source/*.cpp fastlz/fastlz.c)
add_library(${TARGET} 
            ${SOURCES}
            )

add_dependencies(${TARGET} 
                 recastnavigation
                 )

target_include_directories( ${TARGET} PUBLIC 
                            $<BUILD_INTERFACE:Include 
                                              fastlz 
                                              ${CMAKE_INSTALL_PREFIX}/include/recastnavigation>
                            )

target_link_libraries(${TARGET} 
                      Recast 
                      Detour 
                      DetourTileCache
                      )



