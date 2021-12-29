#!/bin/sh 

oldIFS=$IFS
IFS='
'

thisFileDir=$(cd `dirname $0`;pwd)                          # 这个文件目录 build/
pidFile=$thisFileDir/pid

getServerPids="ps -u $LOGNAME |grep Server |grep -v grep |grep -v vim" 

for var in $(echo $getServerPids | bash) 
do 
    pid=$(echo $var |awk '{ printf "%s ", $1 }')
    pname=$(echo $var |awk '{ printf "%s ", $4 }')

    if kill -2 $pid 
    then 
        echo "stop $pname"
    else 
        echo "can't stop $pname"
    fi
done

IFS=$oldIFS

