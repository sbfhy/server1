#!/bin/sh 

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/

cd $thisFileDir

./GameServer & 
echo "start GameServer"
./GateServer &
echo "start GateServer"

