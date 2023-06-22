#include "func.h"


//登录前界面
void get_info_logout(send_info *info)
{    
    memset(info, 0, sizeof(send_info));
    
    while(1) //循环防止用户输入错误指令
    {
        while(getchar() != '\n');  //清空缓冲区

        printf("\n**************登 录 界 面**************\n");
        printf("[ i登录；   r注册；    d注销；  q退出 ]\n");
        printf("***************************************\n");
        printf("请选择：");
        scanf("%c", &info->cmd);

        if(info->cmd == 'i' || info->cmd == 'r'|| info->cmd == 'd')//登录或注册或注销
        {
            printf("请输入账号：");
            scanf("%s", info->id);

            printf("请输入密码：");
            scanf("%s", info->password);

            strcpy(host, info->id);
            strcpy(dest, info->id);

            break;
            
        }else if(info->cmd == 'q') //退出客户端
        {
            exit(0);
        }

    }

    
    return;
}

//登录后界面
void get_info_login(send_info *info)
{    
    memset(info, 0, sizeof(send_info));
    strcpy(info->id, host);
    strcpy(info->dest_id, dest);

    info->cmd = 's'; //发送消息
    scanf("%s", info->buf);

    if(strcmp(info->buf, "connect") == 0) //指定目标
    {
        //改变连接客户端
        scanf("%s", dest);
        strcpy(info->dest_id, dest);

        info->cmd = 'c';

        bzero(info->buf, sizeof(info->buf));

    }else if(strcmp(info->buf, "list") == 0) //查询用户
    {
        info->cmd = 'l';

        bzero(info->buf, sizeof(info->buf));

    }else if(strcmp(info->buf, "exit") == 0) //退出登录
    {
        info->cmd = 'e';

        bzero(info->buf, sizeof(info->buf));
    }
    
    return;
}


void get_info(send_info * const info, const bool * const login, pthread_mutex_t * const lock)
{
    pthread_mutex_lock(lock);

    if(*login == true)
    {
        pthread_mutex_unlock(lock);
        get_info_login(info); //登录后界面

    }else if(*login == false)
    {
        pthread_mutex_unlock(lock);
        get_info_logout(info); //登录前界面
    }

    return;
}


void put_info(recv_info * const info, bool * const login)
{
    *login = info->login; //更新客户端状态

    if(info->type == 's') //服务信息
    {
        printf("%s\n", info->buf);
        
    }else if(info->type == 'c') //客户端信息
    {
        printf("[%s]%s\n", info->send_id, info->buf);
    }

    memset(info, 0, sizeof(recv_info));
    
    return;
}