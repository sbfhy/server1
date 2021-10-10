# rpcserver1

[TOC]

<br />

# 介绍
个人rpcserver尝试整理test1  
[客户端](https://git.xindong.com/huangyong/client1.git)   
[proto](https://git.xindong.com/huangyong/myproto1.git)，要放到myproto目录，并改名myproto/proto/   

<br />

# 服务拆分 
```
C   client  
G   gameserver  
A   gateserver  
```


<br />

# 注意
```
CreateDynamic的派生类一定要显式声明构造函数，否则不会执行注册代码。自己定义的Service和Stub都要显式声明构造函数。  
```

<br />

# 问题记录
## 编译问题
```
libDetourTileCache.a(DetourTileCache.cpp.o)：在函数‘dtTileCache::buildNavMeshTile(unsigned int, dtNavMesh*)’中：DetourTileCache.cpp:(.text+0x15a5)：对‘dtCreateNavMeshData(dtNavMeshCreateParams*, unsigned char**, int*)’未定义的引用。
    添加libnavmesh.a时，要先链接Detour，再链接DetourTileCache。
```


<br />

# 待做
```
RPC超时处理, Delay Response,   
地图多线程导入，  
拆分多服务，  
心跳包，  
```


# 参考
[muduo源码分析（二）-Protobuf RPC流程](https://cloud.tencent.com/developer/article/1400801)  
[C++实现反射(根据类名动态创建对象](https://blog.csdn.net/heyuhang112/article/details/51729435)  

![rpc处理流程.png](类图/rpc处理流程.png)  
![Rpc流程.png](类图/Rpc流程.png){:height=60% width=60%}  


