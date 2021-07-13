# dep 
各个依赖库  
```
dep
|-- cmake-3.19.2-Linux-x86_64
|   |-- bin
|   |-- doc
|   |-- man
|   `-- share
|-- protobuf3.14.0_build
|   |-- bin
|   |-- include
|   `-- lib
`-- REDEME.md
```

<br />

## 1 cmake 
[cmake下载](https://cmake.org/download/)  

## 2 protobuf
1. 下载  
[protobuf-cpp-3.14.0.tar.gz下载](https://github.com/protocolbuffers/protobuf/releases/download/v3.14.0/protobuf-cpp-3.14.0.tar.gz)   
2. 安装依赖工具  
```
sudo apt-get install autoconf automake libtool curl make g++ unzip
```
3. 安装  
```
./configure --prefix=*sourceDir*/dep/protobuf3.14.0_build  
make
make check
make install
```


