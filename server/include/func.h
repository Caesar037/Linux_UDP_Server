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

#include "client_list.h"

typedef struct _recv_info
{
    char cmd; //操作标志（i登录；r注册；d注销；q退出客户端
              //         c指定目标；l查询用户；e退出登录；s发送信息）

    char id[32];
    char password[32];

	char buf[1024]; //接收的消息
	char dest_id[32]; //目标客户端id
    
}recv_info;

typedef struct _send_info
{
    bool login; //host状态（false未上线，true已上线）
    char type; //信息类型（s服务信息；c客户端信息）

    char buf[1024]; //接收的消息
    char send_id[32]; //发送方id

}send_info;

static node* head; //链表头节点
static recv_info r_data; //接收的消息
static send_info s_data; //发送到send方的消息
static send_info d_data; //发送到dest方的消息

extern void init_server(void);
extern void handle_info(const int socket_fd, recv_info * const r_data, send_info * const s_data, send_info * const d_data, struct sockaddr_in * send_addr,  struct sockaddr_in * dest_addr);


#endif