======================
ver1.2   ---- 2022.11.10
======================

设计目标
实现客户端之间通过中转udp服务器完成信息传输

功能描述
客户端启动后进入登录页面，完成注册、注销、退出、登录的功能
登陆后进入登录模式，可以请求服务器返回上线用户信息，连接目标客户端，实现消息互传

设计方案
设计分为两个界面：登录界面，和交互界面（进入登录模式）
用户根据界面提示封装不同的数据包，发送给服务器处理
同时随时接收服务器的数据包并解析，根据信息以不同的需求显示

项目架构

├── bin
│   └── client
├── bmp
│   ├── del.bmp
│   ├── error.bmp
│   ├── exit.bmp
│   ├── init.bmp
│   ├── login.bmp
│   └── success.bmp
├── include
│   ├── client.h
│   ├── func.h
│   ├── get_lcd.h
│   └── get_touch.h
├── lib
├── main
│   ├── main.c
│   └── main.o
├── Makefile
├── readme.md
└── src
    ├── client.c
    ├── client.o
    ├── func.c
    ├── func.o
    ├── get_lcd.c
    ├── get_lcd.o
    ├── get_touch.c
    └── get_touch.o

实现过程
分别启用两个线程：一个负责封装数据并发送，一个负责接收服务器信息并解包
同时在线程间操作共享变量时加上互斥锁，保障页面逻辑稳定性





