#ifndef ETEPOLL_H
#define ETEPOLL_H

#include <unistd.h>
#include <sys/epoll.h>

#include <vector>
#include <iostream>

namespace wzy {

class etEpoll {
public:
    etEpoll(int epollSize = 10000) : epfd(-1), EPOLL_SIZE(epollSize) {
        if(epollSize <= 0) error_handling("error : epoll size less than 0");
    }
    ~etEpoll();
    bool create();
    bool add(int);
    bool del(int);
    bool wait(/*eventlist*/, int timeOut = 3000);
    void close() { ::close(epfd); epfd = -1; };
private:
    int epfd = -1;
    int EPOLL_SIZE = 10000;
    etEpoll();
    static void error_handling(const std::string & message) {
        std::cerr << message << std::endl;
        exit(1);
    }
};

}

/*
*/

#endif