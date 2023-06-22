#include "func.h"

//初始化链表头节点
void init_server()
{
    head = init_list();
}

//登录前的信息处理（send端<—————>server端）
bool handle_info_logout(const int socket_fd, recv_info * const r_data, send_info * const s_data, struct sockaddr_in * send_addr)
{
    //得到发送方节点
    node* s_node = NULL;
    s_node = find_node_id(head, r_data->id);

    switch(r_data->cmd)
    { 
        case 'i': //登录
            if(s_node == NULL) //未注册
            {   
                s_data->login = false;
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "该用户未注册");

            }else if(s_node->login == true) //已登录
            {
                s_data->login = false;
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "该用户已登录");

            }else
            {
                s_node->login = true;
                s_node->client_addr = *send_addr;
                print_list(head); //服务器显示更新的用户信息

                s_data->login = true; //登录完成
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "**************登 录 模 式**************");
            }
            break;

        case 'r': //注册
            if(s_node != NULL) //已注册
            {   
                s_data->login = false; //跳过此设置
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "该用户已注册");

            }else
            {
                insert_node(head, r_data->id, r_data->password, *send_addr);
                print_list(head); //服务器显示更新的用户信息
                
                s_data->login = false; //未登录
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "注册成功");
            }
            break;

        case 'd': //注销
            if(s_node == NULL) //未注册
            {   
                s_data->login = false;
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "该用户未注册");

            }else if(s_node->login == true) //已登录
            {
                s_data->login = false;
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "该用户已登录");

            }else
            {
                delete_node(head, r_data->id, r_data->password);
                print_list(head); //服务器显示更新的用户信息

                s_data->login = false; //未注册
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "注销成功");
            }
            break;

        default:
            return false;
	}
    sendto(socket_fd, s_data, sizeof(send_info), 0, (struct sockaddr *)send_addr, sizeof(struct sockaddr));
    
    return true;
}


//登录后的信息处理（send端<—————>server端<—————>dest端）
bool handle_info_login(const int socket_fd, recv_info * const r_data, send_info * const s_data, send_info * const d_data, struct sockaddr_in * send_addr, struct sockaddr_in * dest_addr)
{
    //得到发送方节点
    node* s_node = NULL;
    s_node = find_node_id(head, r_data->id);

    if(s_node == NULL || s_node->login == false) //未登录
    {   
        s_data->login = true;
        s_data->type = 's'; //服务器信息
        strcpy(s_data->buf, "目标用户丢失");
        
        sendto(socket_fd, s_data, sizeof(send_info), 0, (struct sockaddr *)send_addr, sizeof(struct sockaddr));
    
        return true;

    }

    //得到目标方节点
    node* d_node = NULL;
    d_node = find_node_id(head, r_data->dest_id);

    switch(r_data->cmd)
    {
        case 's': //发送信息
            if(d_node == NULL || d_node->login == false) //未登录
            {   
                s_data->login = true;
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "目标用户丢失");

            }else
            {
                d_data->login = true;
                d_data->type = 'c'; //客户端信息
                strcpy(d_data->buf, r_data->buf);
                strcpy(d_data->send_id, r_data->id);

                sendto(socket_fd, d_data, sizeof(send_info), 0, (struct sockaddr *)&d_node->client_addr, sizeof(struct sockaddr));

                s_data->login = true;
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "发送成功");
            }
            break;

        case 'c': //连接用户
            if(d_node == NULL || d_node->login == false) //未登录
            {   
                s_data->login = true;
                s_data->type = 's'; //服务器信息
                strcpy(s_data->buf, "目标用户不存在");

            }else
            {
                d_data->login = true;
                d_data->type = 'c'; //客户端信息
                strcpy(d_data->buf, "连接已建立");
                strcpy(d_data->send_id, r_data->id);

                sendto(socket_fd, d_data, sizeof(send_info), 0, (struct sockaddr *)&d_node->client_addr, sizeof(struct sockaddr));

                s_data->login = true;
                s_data->type = 'c'; //服务器信息
                strcpy(s_data->send_id, r_data->dest_id);
                strcpy(s_data->buf, "连接已完成");
            }
            break;
        
        case 'l': //查询用户（直接遍历链表，依次返回节点信息）
        {
            //定义一个大结构体的指针
            node* p = NULL;
            //定义一个内核结构体指针
            struct list_head* pos = NULL;

            s_data->login = true;
            s_data->type = 's'; //服务器信息
            strcpy(s_data->buf, "\n**************用 户 信 息**************");
            sendto(socket_fd, s_data, sizeof(send_info), 0, (struct sockaddr *)send_addr, sizeof(struct sockaddr));
                   
            list_for_each(pos, &head->list)
            {
                p = list_entry(pos, node, list);

                memset(s_data, 0, sizeof(send_info));
                s_data->login = true;
                s_data->type = 's'; //服务器信息
                sprintf(s_data->buf, "client->id: %s login: %s", p->id, (p->login)?"true":"false");
                
                sendto(socket_fd, s_data, sizeof(send_info), 0, (struct sockaddr *)send_addr, sizeof(struct sockaddr));
            }

            memset(s_data, 0, sizeof(send_info));
            s_data->login = true;
            s_data->type = 's'; //服务器信息
            strcpy(s_data->buf, "\n请选择用户：connect <id>");

            break;
        }

        case 'e': //退出登录

            s_node->login = false;
            print_list(head); //服务器显示更新的用户信息

            s_data->login = false; //未登录
            s_data->type = 's'; //服务器信息
            strcpy(s_data->buf, "退出成功");

            break;

        default:
            return false;
    }
    sendto(socket_fd, s_data, sizeof(send_info), 0, (struct sockaddr *)send_addr, sizeof(struct sockaddr));
    
    return true;
}

//整合处理函数
void handle_info(const int socket_fd, recv_info * const r_data, send_info * const s_data, send_info * const d_data, struct sockaddr_in * send_addr,  struct sockaddr_in * dest_addr)
{    
    memset(s_data, 0, sizeof(send_info));
    memset(d_data, 0, sizeof(send_info));

    //优化提速
    if(handle_info_logout(socket_fd, r_data, s_data, send_addr) == false)
        handle_info_login(socket_fd, r_data, s_data, d_data, send_addr, dest_addr);
        
    memset(r_data, 0, sizeof(recv_info));

    return;
}