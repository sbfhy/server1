include(ExternalProject)


# recastnavigation
ExternalProject_Add(recastnavigation
    GIT_REPOSITORY https://github.com/sbfhy/recastnavigation.git
    GIT_SHALLOW true
    # GIT_TAG origin/master
    # BUILD_ALWAYS true
    PREFIX recastnavigation
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DRECASTNAVIGATION_DEMO=OFF -DRECASTNAVIGATION_TESTS=OFF -DRECASTNAVIGATION_EXAMPLES=OFF
    # INSTALL_DIR ${CMAKE_INSTALL_PREFIX}               # 这一行没用，必须加-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} ??? 不加第一次编译时会报错
    # INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install       # 等同于手动 make install
    # INSTALL_COMMAND ""
)


# physx
if (CMAKE_BUILD_TYPE)
    set(PHYSX_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()
ExternalProject_Add(physx
    GIT_REPOSITORY https://github.com/sbfhy/PhysX.git
    GIT_SHALLOW true
    GIT_TAG origin/4.1
    PREFIX physx
    # BUILD_ALWAYS true
    # CMAKE_ARGS -DCMAKE_BUILD_TYPE=${PHYSX_BUILD_TYPE} -Wno-error=class-memaccess 
    UPDATE_COMMAND ""                                   # 不加这个会刷新文件，每次都会重编，但加了git不会更新
    CONFIGURE_COMMAND ""                                # 因为主目录没有CMakeLists.txt，必须加上这一行以覆盖默认的配置命令  
    # 编译类型 linux-debug
    BUILD_COMMAND  bash ../physx/physx/generate_projects.sh linux && cd ../physx/physx/compiler/linux-${PHYSX_BUILD_TYPE}/ && make -j$(nproc)  
    # BUILD_COMMAND  cd ../physx/physx/compiler/linux-${PHYSX_BUILD_TYPE}/ && make -j$(nproc)  
    # BUILD_COMMAND  ""  
    # BUILD_IN_SOURCE true
    INSTALL_COMMAND ""
)
