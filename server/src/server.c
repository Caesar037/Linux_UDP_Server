#include "server.h"

int server()
{   
    //初始化（链表）
    init_server();

    struct sockaddr_in dest_addr; //接收目标IP地址和端口
    struct sockaddr_in send_addr; //接受客户端的IP地址和端口
    socklen_t addrlen = sizeof(send_addr);

    //建立套接字
    //AF_INET-->ipv4  SOCK_DGRAM-->udp
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd == -1)
    {
        perror("socket");
        return -1;
    }

    //填充本机IP地址和端口（服务器）
    struct sockaddr_in host_addr;
    host_addr.sin_family = AF_INET; //ipv4
    host_addr.sin_port = htons(HOST_PORT); //本机端口号转网络端口号
    host_addr.sin_addr.s_addr = inet_addr(HOST_ADDR); //本机IP转换为网络IP

    //绑定本机IP和端口（服务器）
    if(bind(socket_fd, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1)
    {
        perror("bind");
        exit(-1);
    }
    printf("bind success!\n");


    //host_addr服务器地址；dest_addr目标方地址；send_addr发送方地址
    while(1)
    {
        memset(&dest_addr, 0, sizeof(struct sockaddr_in));
        memset(&send_addr, 0, sizeof(struct sockaddr_in));
        
        //接受数据
        recvfrom(socket_fd, &r_data, sizeof(recv_info), 0, (struct sockaddr *)&send_addr, &addrlen);

        //处理并发送到目标客户端(套接字 + 接收&目标&发送数据包 + 接收&目标地址)
        handle_info(socket_fd, &r_data, &s_data, &d_data, &send_addr, &dest_addr);
    }

    close(socket_fd);
    return 0;
}
