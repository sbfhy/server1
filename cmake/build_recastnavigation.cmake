include(ExternalProject)

ExternalProject_Add(recastnavigation
    GIT_REPOSITORY https://github.com/sbfhy/recastnavigation.git
    GIT_SHALLOW true
    # BUILD_ALWAYS true
    PREFIX recastnavigation
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}  -DRECASTNAVIGATION_DEMO=OFF -DRECASTNAVIGATION_TESTS=OFF -DRECASTNAVIGATION_EXAMPLES=OFF
    # INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install       # 等同于手动 make install
    # INSTALL_COMMAND ""
)

