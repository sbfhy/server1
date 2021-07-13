#!/bin/bash
date;
echo "ctags done......"
find -L . \( -path ./build -o -path ./bin -o -path ./dep -o -path ./DesignerWork -o -path ./src/Robots \) -prune -type f -o -name  "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.py" -o -name "*.cmake" -o -name "*.proto" -o -name "*.lua" -o -name "*.hpp" > cscope.files;
# find -L . -name "*.pl" >>cscope.files

date;
cscope -RCbq -i cscope.files;
ctags -R -L cscope.files;
echo "cscope done......"
date;
