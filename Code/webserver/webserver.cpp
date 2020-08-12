#include "webserver.h"

namespace wzy {

webserver::webserver(
    size_t port,
    size_t workThreadNum,
    size_t taskQueueNum,
    size_t epollSize,
    size_t maxFd,
    int backlog,
    std::string homePage
) : PORT(port),  WORK_THREAD_NUM(workThreadNum), TASK_QUEUE_NUM(taskQueueNum),
    EPOLL_SIZE(epollSize), MAX_FD(maxFd),
    MAX_BACKLOG(backlog), HOME_PAGE(homePage),
    ser(), clnt() {
    // new epoll
    ep = std::make_shared<wzy::etEpoll>(EPOLL_SIZE);
    // new threadPool
    thrdpool = std::make_shared< threadPool<httpcon> >(WORK_THREAD_NUM, TASK_QUEUE_NUM);
    // 如果是相对路径
    if(HOME_PAGE.empty() || HOME_PAGE[0] != '/') {
        char buffer[200] = {0};
        getcwd(buffer, sizeof(buffer));
        std::string tmp(buffer);
        HOME_PAGE = tmp + HOME_PAGE;
    }

    // 空或者末尾没有 '/'
    if(HOME_PAGE.empty() || HOME_PAGE.back() != '/')
        HOME_PAGE += "/";
    
    // new http 对象 待
}

webserver::~webserver() = default;

void webserver::listen() {
    ser.socket();
    ser.bind(PORT);
    ser.listen(MAX_BACKLOG);
    ser.setNonBlocking();
}

void webserver::dealClientConnction() {
    while(ser.nonBlockingAccept(clnt)) {
        if(clientCount >= static_cast<int>(MAX_FD) ) {
            error_handling("server busy", false);
            break;
        }
        clnt.setNonBlocking();
        if(ep -> add(clnt, EPOLLIN|EPOLLET|EPOLLOUT)) {
            std::cout << "connected client : " << clnt << std::endl;
            ++clientCount;
        }
    }
}

void webserver::start() {
    isStop = true;
    ep -> create();
    ep -> add(ser, EPOLLIN|EPOLLET);
    while(!isStop) {
        int cnt = ep -> wait(-1);
        if(cnt == -1) break;
        for(int i = 0; i < cnt; ++i) {
            int sockfd = ep -> events[i].data.fd;
            if(sockfd == ser) {
                dealClientConnction();
            }
            else if( (ep -> events[i].events) & EPOLLIN) {
                dealRead(sockfd);
            }
            else if( (ep -> events[i].events) & EPOLLOUT) {
                dealWrite(sockfd);
            }
        }
    }
}

}