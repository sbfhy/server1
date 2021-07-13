#!/bin/bash

set -x                                                      # 设置在运行结果之前，先输出执行的那一行命令。

buildDir=$(cd `dirname $0`;pwd)                             # 编译目录，即这个文件目录 build/
projectDir=$buildDir/..                                     # 项目根目录
buildType=debug                                             # 编译类型
buildTypeDir=$buildDir/$buildType                           # build/debug目录
installDir=${buildTypeDir}/install                          # install命令的前缀目录，build/debug/install

if [ -f $buildTypeDir/CMakeCache.txt ]; then
  rm $buildTypeDir/CMakeCache.txt                           # 暂时先每次删掉CMakeCache.txt
fi

# if [ -d $buildDir/pb ]; then
#   rm -rf $buildDir/pb                                       # 暂时每次删掉build/pb目录
# fi

export PATH=$projectDir/dep/cmake-3.19.2-Linux-x86_64/bin/:$PATH  # 使用项目里自带的cmake

CXX=${CXX:-g++}

mkdir -p $buildTypeDir/bin/log
mkdir -p $buildDir/pb

cd $buildTypeDir                                  \
  && cmake                                        \
           -DCMAKE_BUILD_TYPE=$buildType          \
           -DCMAKE_INSTALL_PREFIX=$installDir     \
           $projectDir                            \
  && make -j$(nproc) $*                           \
  # && make install
 
# proto 测试
# $projectDir/dep/protobuf3.14.0_build/bin/protoc --plugin=protoc-gen-rpc=$buildTypeDir/bin/protoc-gen-rpc --cpp_out=$buildDir/pb --rpc_out=$buildDir/pb    $projectDir/myproto1/rpcservice.proto  -I$projectDir/myproto1 
# $projectDir/dep/protobuf3.14.0_build/bin/protoc --plugin=protoc-gen-rpc=$buildTypeDir/bin/protoc-gen-rpc --cpp_out=$buildDir/pb --rpc_out=$buildDir/pb    $projectDir/myproto1/sudoku.proto  -I$projectDir/myproto1 

# $projectDir/dep/protobuf3.14.0_build/bin/protoc  --cpp_out=$buildDir/pb  $projectDir/myproto1/sudoku.proto   -I$projectDir/myproto1 
# $projectDir/dep/protobuf3.14.0_build/bin/protoc  --cpp_out=$buildDir/pb  $projectDir/myproto1/rpcservice.proto   -I$projectDir/myproto1 


