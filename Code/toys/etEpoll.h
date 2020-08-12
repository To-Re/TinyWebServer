#ifndef ETEPOLL_H
#define ETEPOLL_H

#include <unistd.h>
#include <sys/epoll.h>

#include <iostream>

namespace wzy {

class etEpoll {
public:
    // epollSize 事件接收范围
    etEpoll(size_t epollSize = 10000) : epfd(-1), EPOLL_SIZE(epollSize) {
        if(epollSize <= 0)
            error_handling("error : epoll size less than 0");
        events = new epoll_event[epollSize];
    }

    ~etEpoll() { close(); }

    // 创建 epoll 文件描述符
    void create();

    // addfd 需要注册的文件描述符
    // 注册事件可用或运算，EPOLLIN 需要读取数据，EPOLLET 边缘触发
    // 成功返回 ture，否则返回 false
    bool add(const int & addfd, const uint32_t & event);

    // delfd 需要删除的文件描述符
    // 成功返回 ture，否则返回 false
    bool del(const int & delfd);

    // timeOut 超时时间单位毫秒，默认 -1 阻塞
    // 返回发生事件的数量，-1 调用出错，超时返回 0
    int wait(int timeOut = -1);

private:
    int epfd = -1;
    int EPOLL_SIZE = 10000;
    static void error_handling(const std::string & message, const bool & isexit = true) {
        std::cerr << message << std::endl;
        if(isexit) exit(1);
    }
    void close() { ::close(epfd); epfd = -1; delete []events; };
public:
    // 发生事件文件描述符集合
    struct epoll_event *events;
};

}

#endif