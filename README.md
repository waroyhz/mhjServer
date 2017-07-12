# mhjServer
美好家1.0通讯服务器，基于spServer领导者／追随者模式。

# 目录说明

MHJDeviceServer：设备服务端，处理协议接收，将协议转发给目标。

MHJServerShared：服务器共用工程，本类和调度服务端共用，目录内协议申明服务端、客户端共用。

MHJServerSharedMySql：服务器MySql客户端，还有一个是基于Sqlite的，是客户端用的。

MHJSpserverLib：spServer代码集成，因原官网上的代码有几处小问题，在这里做了一些修正和扩充。

# 实现功能

1、支持Tcp直接连接到服务端，保持长链接，保证实时性。用户通过手机遥控硬件，服务器设计响应要求不超过50ms。

2、Tcp数据包加密，采用256位异或加密，每个设备都有自己的256位密钥，同时每次会话还会产生临时密钥，保证数据交互安全。

3、设备密钥储存在mysql服务器，访问数据库中间件采用ODB（ODB项目地址：http://www.codesynthesis.com/products/odb ）。

4、底层和协议定义文件共用，减少其他客户端的底层调试工作量，底层移植到IOS、Android平台。