#!/bin/sh 

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/
cd $thisFileDir

rm ./log/* -f 
rm ./core* -f 
