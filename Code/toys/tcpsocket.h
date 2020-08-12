#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>
#include <cstddef>
#include <iostream>

namespace wzy {
 // 不实现客户端的连接了，服务器也不需要

class tcpSocket{
public:
    tcpSocket() : sockfd(-1) {}
    ~tcpSocket() { close(); }
    void setsockfd(int fd) { sockfd = fd; }
    void socket();
    
    // 端口号
    void bind(int);
    
    // 请求等待队列长度
    void listen(int backlog = 100);
    
    // 取出一个 TCP 连接
    void accept(tcpSocket &);

    // 设置为非阻塞
    void setNonBlocking() {
        // 看不懂，这行代码作用是将 sockfd 改为非阻塞
        fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
    }

    // 非阻塞取出一个 TCP 连接
    // 成功返回 ture，否则返回 false
    bool nonBlockingAccept(tcpSocket &);

    // 关闭连接
    void close() { ::close(sockfd); sockfd = -1;}
    
    // 输入输出有点麻烦，直接整个隐式转换，以后想到更好的封装再说
    operator int() const { return sockfd; }
private:
    int sockfd = -1;

    static void error_handling(const std::string & message) {
        std::cerr << message << std::endl;
        exit(1);
    }
};

}

#endif