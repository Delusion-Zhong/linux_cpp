#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
using namespace std;

int main()
{
    //! 1. 创建监听的 socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        return -1;
    }

    //! 2. 绑定本地 ip 和 port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    /* 两个结构体，长度都为16字节，sockaddr_in.sin_family的数据存入sockaddr.sa_family,
        剩下的14个字节存入sockaddr.sa_data,
        这样在各种操作中可以方便的处理端口号和ip地址；
     */
    int ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        close(fd);
        return -1;
    }

    // 设置监听
    ret = listen(fd, 128);
    if (ret == -1)
    {
        perror("listen");
        close(fd);
        return -1;
    }

    // 阻塞并等待客户端连接
    struct sockaddr_in caddr;
    socklen_t addrlen = sizeof(caddr);

    int aept = accept(fd, (struct sockaddr *)&caddr, &addrlen);
    if (aept == -1)
    {
        perror("accept");
        close(fd);
        return -1;
    }

    // 连接建立成功，打印客户端的ip和端口信息
    cout << "cilent 连接成功！！！！" << endl;
    char ip[32];
    cout << "客户端的ip: " << inet_ntop(AF_INET, &caddr.sin_addr, ip, sizeof(ip))
         << ", 端口: " << ntohs(caddr.sin_port) << endl;

    while (1)
    {
        char buff[1024];
        string server = "Holle,cilent ,my is  server......";
        // 调用 recv 函数从连接的客户端 (aept) 接收数据
        int len = recv(aept, buff, sizeof(buff), 0);
        if (len > 0)
        {
            cout << "服务器 接收到数据: " << buff << endl;

            // 在发送之前检查发送长度
            // 调用 send 函数将接收到的数据发送回客户端。
            if (send(aept, server.c_str(), server.length(), 0) < 0)
            {
                perror("send");
                break;
            }
        }
        else if (len == 0)
        {
            cout << "client 断开连接......" << endl;
            break;
        }
        else
        {
            perror("recv");
            break;
        }
    }

    // 关闭连接之前确保断开连接
    close(aept);
    close(fd);

    return 0;
}
