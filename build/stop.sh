#!/bin/sh 

oldIFS=$IFS
IFS='
'

getServerPids="ps -aux |grep $LOGNAME | grep Server | grep -v grep | grep -v vim |grep -v \"\-d\" "   # $LOGNAME是用户名，这行筛选出所有Server进程

for var in $(echo $getServerPids | bash) 
do 
    pid=$(echo $var |awk '{ printf "%s ", $2 }')
    pname=$(echo $var |awk '{ print substr($11,3) }')

    if kill -2 $pid 
    then 
        echo "stop $pname"
    else 
        echo "can't stop $pname"
    fi
done

IFS=$oldIFS

