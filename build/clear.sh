#!/bin/sh 

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/

source $thisFileDir/config_build
cd $thisFileDir/$buildType/bin/

rm ./log/* -f 
rm ./core* -f 
