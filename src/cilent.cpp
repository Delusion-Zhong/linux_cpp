#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
using namespace std;

int main()
{
    //! 1. 创建通讯的 socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        return -1;
    }

    //! 2.连接 ip 和 port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);

    // 将字符串形式的 IP 地址转换为网络字节序并存储在 saddr.sin_addr.s_addr 中。
    inet_pton(AF_INET, "192.168.116.15", &saddr.sin_addr.s_addr);

    // saddr.sin_addr.s_addr = INADDR_ANY;
    //! 连接服务器
    int ret = connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("connect");
        return -1;
    }
    //! 3. 通讯
    int num = 1;
    while (1)
    {
        // 发送数据
        string str = "你好呀,我是来至于 cilent 的消息............";

        send(fd, str.c_str(), str.length(), 0);

        // 接收数据
        char buff[1024];

        memset(buff, 0, sizeof(buff)); // 将 buff 中的数据清空

        int len = recv(fd, buff, sizeof(buff), 0);
        if (len > 0)
        {
            cout << "客户端 接收到数据:" << buff << endl;
        }
        else if (len == 0)
        {
            cout << "server  断开连接......" << endl;
            break;
        }
        else
        {
            perror("recv");
            break;
        }
        sleep(1);
    }

    // 关闭连接之前确保断开连接
    close(fd);

    return 0;
}
