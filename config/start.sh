#!/bin/sh 

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/

cd $thisFileDir

./GameServer 127.0.0.1 & 
./GateServer 127.0.0.1 &

cd -
