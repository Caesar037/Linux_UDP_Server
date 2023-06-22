# UDP协议的聊天系统
基于UDP的聊天软件，纯C语言编写（使用时记得修改IP地址）

## 客户端设计

实现客户端之间通过中转udp服务器完成信息传输

### 功能描述

客户端启动后进入登录页面，完成注册、注销、退出、登录的功能
登陆后进入登录模式，可以请求服务器返回上线用户信息，连接目标客户端，实现消息互传

### 设计方案

设计分为两个界面：登录界面，和交互界面（进入登录模式）
用户根据界面提示封装不同的数据包，发送给服务器处理
同时随时接收服务器的数据包并解析，根据信息以不同的需求显示

### 实现过程

分别启用两个线程：一个负责封装数据并发送，一个负责接收服务器信息并解包
同时在线程间操作共享变量时加上互斥锁，保障页面逻辑稳定性

<br/>

## 服务器设计

服务器接收客户端发送的数据包，解析数据包，根据客户端需求配置服务器发送数据包并发送给对应客户端

### 功能描述

服务器接收客户端发送的数据包并进行解析
根据客户端需求配置服务器发送数据包并发送给对应客户端
更新显示服务器数据表

### 设计方案

服务器绑定固定地址，接收任意客户端数据包和地址，解析命令根据，
根据当前客户端状态判断客户端权限，未登录客户端只能和服务器交换信息，
登录后的客户端即可以与服务器交互，也可以要求服务器信息转发到其他客户端

### 实现过程

接收客户端信息和地址，判断是否存在于服务器链表中
节点存在则正常登录，对应的客户端地址更新到对应用户节点
登录后的节点可以查找其他客户端节点并通过服务器转发信息
