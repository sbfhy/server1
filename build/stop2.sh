#!/bin/sh 

oldIFS=$IFS
IFS='
'


thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/
pidFile=$thisFileDir/pid

while read pid
do 

    if kill -2 $pid 
    then 
        echo "stop $pid"
    else 
        echo "can't stop $pid"
    fi
done < $pidFile


cat /dev/null > $pidFile


IFS=$oldIFS

