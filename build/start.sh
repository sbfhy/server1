#!/bin/sh 

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/
pidFile=$thisFileDir/pid

source $thisFileDir/config_build
cd $thisFileDir/$buildType/bin/

mkdir -p log 

cat /dev/null > $pidFile

./GameServer &
echo $! >> $pidFile
echo "start GameServer"

./GateServer &
echo $! >> $pidFile
echo "start GateServer"

