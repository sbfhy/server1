#!/bin/sh 

oldIFS=$IFS
IFS='
'

getServerPids="ps -u $LOGNAME | grep Server | grep -v grep | grep -v vim"   # $LOGNAME是用户名，这行筛选出所有Server进程

for var in $(echo $getServerPids | bash) 
do 
    pid=$(echo $var |awk '{ printf "%s ", $1}')
    pname=$(echo $var |awk '{ printf "%s ", $4}')

    if kill -2 $pid 
    then 
        echo "stop $pname"
    else 
        echo "can't stop $pname"
    fi
done

IFS=$oldIFS
