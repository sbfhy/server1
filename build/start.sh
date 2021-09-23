#!/bin/sh 

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/

source $thisFileDir/config_build
cd $thisFileDir/$buildType/bin/

./GameServer & 
echo "start GameServer"
./GateServer &
echo "start GateServer"

