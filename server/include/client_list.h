#ifndef _CLIENT_LIST_H_
#define _CLIENT_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "kernel_list.h"


typedef struct _node
{
    bool login; //登录标志

    char id[64];
    char password[64];

    struct sockaddr_in client_addr;

    struct list_head list; //内核的小结构体

}node;

extern node* init_list();
extern int destory_list(node* head);
extern int print_list(node* head);

extern int insert_node(node* head, char* id, char* password, struct sockaddr_in client_addr);
extern int delete_node(node* head, char* id, char* password);
extern node* find_node(node* head, char* id, char* password);
extern node* find_node_id(node* head, char* id);

#endif