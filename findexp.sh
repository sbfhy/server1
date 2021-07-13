#!/bin/sh

# set -x

sourceDir=`dirname $0`
if [ $# -gt 1 ]; then
  sourceDir=$2
  echo "sourceDir:$sourceDir,"", 参数个数:$#"
fi


grep --color --exclude-dir=dep -nrw --include=*.{proto,cmake,txt,sh}  "$1" $sourceDir
echo ""


function findSouce() {
  grep --color -nrw --exclude-dir={dep,build,tests} --include=*.{cc,cpp,lua,c} --exclude=*.{pb.h,pb.cc} "$1" "$2"
  echo ""
  grep --color -nwr --exclude-dir={dep,build,tests} --include=*.h --exclude=*.{pb.h,pb.cc} "$1" "$2"
  echo ""
}

findSouce "$1" "${sourceDir}"



