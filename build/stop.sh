#!/bin/sh 

oldIFS=$IFS
IFS='
'

getServerPids="ps ux |grep Server |grep -v grep |grep -v vim" 

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

