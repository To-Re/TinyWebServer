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
    ser(), clnt(),
    // new httpcon
    clnthttp(new httpcon[MAX_FD], [](httpcon *p){ delete[]p; }) {
    // new epoll
    ep = std::make_shared<wzy::etEpoll>(EPOLL_SIZE);
    // new threadPool
    thrdpool = std::make_shared< threadPool<std::function<void()> > >(WORK_THREAD_NUM, TASK_QUEUE_NUM);

    // 获得 Home_Page 绝对路径
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
    
    // 初始化 httpcon
    for(size_t i = 0; i < MAX_FD; ++i) {
        clnthttp[i].init(ep, HOME_PAGE);
    }
}

webserver::~webserver() = default;

void webserver::listen() {
    ser.socket();
    ser.bind(PORT);
    ser.listen(MAX_BACKLOG);
    ser.setNonBlocking();
    std::cout << "listen : " << PORT << std::endl;
}

void webserver::dealClientConnction() {
    while(ser.nonBlockingAccept(clnt)) {
        // clientCount 在 httpcon 中维护
        if(httpcon::clntCount >= static_cast<int>(MAX_FD) ) {
            error_handling("server busy", false);
            break;
        }
        clnt.setNonBlocking();
        // clnthttp[clnt].closeCon();
        if(clnthttp[clnt].startCon(clnt)) {
            std::cout << "connected client : " << clnt << std::endl;
        }
        else {/* 已存在 httpcon ，应该不会发生，发生了如果频率还大属于比较严重的错误*/}
    }
}

void webserver::dealRead(int sockfd) {
    thrdpool->addTask( std::bind(&httpcon::read, &clnthttp[sockfd]));
}

void webserver::dealWrite(int sockfd) {
    thrdpool->addTask( std::bind(&httpcon::write, &clnthttp[sockfd]) );
}

void webserver::start() {
    isStop = false;
    ep -> create();
    ep -> add(ser, EPOLLIN|EPOLLET);
    while(!isStop) {
        int cnt = ep -> wait(-1);
        // std::cout << "信号数量 ： " << cnt << std::endl;
        if(cnt == -1) break;
        for(int i = 0; i < cnt; ++i) {
            int sockfd = ep -> events[i].data.fd;
            if(sockfd == ser) {
                dealClientConnction();
            }
            // 优先处理读信号，读信号会触发写信号
            else if( (ep -> events[i].events) & EPOLLIN) {
                std::cout << "dealRead" << std::endl;
                dealRead(sockfd);
            }
            /*
                一个细节 epoll 刚注册信号 EPOLLOUT，会直接触发，因为是可写
                我说怎么还没接收消息，就回复了。。
            */
            else if( (ep -> events[i].events) & EPOLLOUT) {
                std::cout << "dealWrite" << std::endl;
                dealWrite(sockfd);
            }
        }
    }
}

}