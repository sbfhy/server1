#!/bin/sh 

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/

cd $thisFileDir

./stop.sh
./start.sh
