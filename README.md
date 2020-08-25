## Windows安全防火墙

这是2008年开发的一个Windows安全组件，主要包括两个部分，一是内核驱动防火墙，通过白名单机制，阻止未授权的驱动的加载，并且通过HOOK SSDT的方式保护自身进程的安全，二是ARP防火墙，它通过NDIS中间层驱动捕获以太数据报文，过滤掉其中的恶意ARP报文。自夸一下，当年项目代码很工整，并且自定义了C++ stub，用于实现ddk下编译C++代码 ：）

drv文件夹：  
DrvShield文件夹：驱动防火墙内核模块  
kdarpshd文件夹：ARP防火墙内核模块  

exe文件夹：  
DrvShield文件夹：主执行程序  
MakePolicyFile文件夹：策略白名单制作工具