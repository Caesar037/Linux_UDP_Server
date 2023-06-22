#ifndef _SERVER_H_
#define _SERVER_H_

#include "func.h"

#define HOST_PORT 60000 //本机的端口号
#define HOST_ADDR "192.168.11.121" //本机的IP地址

static int socket_fd;

extern int server(void);

#endif