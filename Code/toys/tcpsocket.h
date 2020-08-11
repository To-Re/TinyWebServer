#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>
#include <cstddef>

namespace wzy {

// 不实现客户端的连接了，服务器也不需要
class tcpSocket{
public:
    tcpSocket() : sockfd(-1) {}
    ~tcpSocket() { Close(); }
    void setsockfd(int fd) { sockfd = fd; }
    bool Socket();
    bool Bind(int);
    bool Listen(int backlog = 100);
    bool Accept(tcpSocket &);
    void Close() { close(sockfd); sockfd = -1;}
    // 输入输出有点麻烦，直接整个隐式转换，以后想到更好的封装再说
    operator int() const { return sockfd; }
private:
    int sockfd = -1;
};

}

#endif