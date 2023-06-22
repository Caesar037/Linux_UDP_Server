#include "client.h"

void *send_thread(void *arg)
{
    //服务器的IP地址和端口
    struct sockaddr_in server_addr = *(struct sockaddr_in *)arg;

    while(1)
    {
        //处理发送数据包
        get_info(&s_data, &login, &lock); //需要传锁，通过全局状态（是否登录）判断页面选择
        sendto(socket_fd, &s_data, sizeof(send_info), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }

    pthread_exit(NULL);
}

void *recv_thread(void *arg)
{
    while(1)
    {
        sleep(0.1); //防止抢锁太快
        pthread_mutex_lock(&lock);

        recvfrom(socket_fd, &r_data, sizeof(recv_info), 0, NULL, NULL);
        put_info(&r_data, &login); //处理接收数据包并输出，更新全局状态（是否登录）

        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}


int client(void)
{
    //初始化客户端数据
    socket_fd = -1;
    login = false;

    memset(&s_data, 0, sizeof(send_info));
    memset(&r_data, 0, sizeof(recv_info));

    pthread_mutex_init(&lock, NULL);

    pthread_t tid1;
    pthread_t tid2;

            
    //1.建立套接字
    //AF_INET-->ipv4  SOCK_DGRAM-->udp
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd == -1)
    {
        perror("socket");
        return -1;
    }

    // //2.填充本机IP地址和端口，测试用
    // struct sockaddr_in host_addr;
    // host_addr.sin_family = AF_INET; //ipv4
    // host_addr.sin_port = htons(HOST_PORT); //本机端口号转网络端口号
    // host_addr.sin_addr.s_addr = inet_addr(HOST_ADDR); //本机IP转换为网络IP

    // //绑定本机IP和端口
    // if(bind(socket_fd, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1)
    // {
    //     perror("bind");
    //     exit(-1);
    // }
    // printf("bind success!\n");

    //3.填充服务器IP地址和端口
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; //ipv4
    server_addr.sin_port = htons(SERVER_PORT); //本机端口号转网络端口号
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR); //本机IP转换为网络IP


    printf("==============欢 迎 使 用==============\n \
            按回车健进入");
    pthread_create(&tid2, NULL, send_thread, (void *)&server_addr);
    pthread_create(&tid1, NULL, recv_thread, (void *)&server_addr);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&lock); //销毁互斥锁
    close(socket_fd);
    return 0;
}
