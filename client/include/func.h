#ifndef _FUNC_H_
#define _FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

typedef struct _send_info
{
    char cmd; //操作标志（i登录；r注册；d注销；q退出客户端
              //         c指定目标；l查询用户；e退出登录；s发送信息）

    char id[32];
    char password[32];

	char buf[1024]; //发送的消息
	char dest_id[32]; //目标客户端id
    
}send_info;

typedef struct _recv_info
{
    bool login; //host状态（false未上线，true已上线）
    char type; //信息类型（s服务信息；c客户端信息）

    char buf[1024]; //接收的消息
    char send_id[32]; //发送方id

}recv_info;

static send_info s_data;
static recv_info r_data;

static char host[32]; //本机客户端id
static char dest[32]; //目标客户端id

extern void get_info(send_info * const info, const bool * const login, pthread_mutex_t * const lock);
extern void put_info(recv_info * const info, bool * const login);


#endif