#ifndef WEBSERVER_H
#define WEBSERVER_H


#include <memory>
#include <iostream>
#include <functional>

#include "config.h"
#include "etEpoll.h"
#include "tcpsocket.h"
#include "threadpool.h"
#include "httpcon.h"

namespace wzy {

class webserver {
public:
    webserver(
        size_t port = config::PORT,
        size_t workThreadNum = config::WORK_THREAD_NUM,
        size_t taskQueueNum = config::TASK_QUEUE_NUM,
        size_t epollSize = config::EPOLL_SIZE,
        size_t maxFd = config::MAX_FD,
        int backlog = config::MAX_BACKLOG,
        std::string homePage = config::HOME_PAGE
    );
    ~webserver();
    void listen();
    void start();
private:
    static void error_handling(const std::string & message, const bool & isexit = true) {
        std::cerr << message << std::endl;
        if(isexit) exit(1);
    }
    void dealClientConnction();
    void dealRead(int);
    void dealWrite(int);

private:
    const size_t PORT;
    const size_t WORK_THREAD_NUM;
    const size_t TASK_QUEUE_NUM;
    const size_t EPOLL_SIZE;
    const size_t MAX_FD;
    const int MAX_BACKLOG;
    std::string HOME_PAGE;

    // webserver 是否运行
    bool isStop = false;
    // 套接字
    wzy::tcpSocket ser, clnt;
    // epoll
    std::shared_ptr<etEpoll> ep;
    // 线程池
    std::shared_ptr<threadPool<std::function<void()> > > thrdpool;
    // httpcon
    std::shared_ptr<httpcon[]> clnthttp;
};

}

#endif