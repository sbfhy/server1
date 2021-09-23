#!/bin/bash

set -x                                                      # 设置在运行结果之前，先输出执行的那一行命令。

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/
source $thisFileDir/config_build                            # 读取编译类型 build or release
projectDir=$thisFileDir/..                                  # 项目根目录
buildTypeDir=$thisFileDir/$buildType                        # build/debug目录
installDir=${buildTypeDir}/install                          # install命令的前缀目录，build/debug/install

# if [ -f $buildTypeDir/CMakeCache.txt ]; then
#   rm $buildTypeDir/CMakeCache.txt                         # 暂时先每次删掉CMakeCache.txt
# fi

# if [ -d $thisFileDir/pb ]; then
#   rm -rf $thisFileDir/pb                                  # 暂时每次删掉build/pb目录
# fi

export PATH=$projectDir/dep/cmake-3.19.2-Linux-x86_64/bin/:$PATH  # 使用项目里自带的cmake

CXX=${CXX:-g++}

mkdir -p $buildTypeDir/bin/log

cd $buildTypeDir                                  \
  && cmake                                        \
           -DCMAKE_BUILD_TYPE=$buildType          \
           -DCMAKE_INSTALL_PREFIX=$installDir     \
           $projectDir                            \
  && make -j$(nproc) $*                           \
#   && make install
