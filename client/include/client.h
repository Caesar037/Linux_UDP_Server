#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "func.h"

//#define HOST_PORT 20001 //本机的端口号，测试用
//#define HOST_ADDR "192.168.11.121" //本机的IP地址，测试用

#define SERVER_PORT 60000 //服务器的端口号
#define SERVER_ADDR "192.168.11.121" //服务器的IP地址


static pthread_mutex_t lock; //用于同步互斥的互斥锁
static int socket_fd;
static bool login; //登录标志


extern int client(void);


#endif