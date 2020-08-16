#include "httpcon.h"

namespace wzy {

int httpcon::clntCount = 0;

httpcon::httpcon(int inBuffSize, int outBuffSize)
  : in(inBuffSize), out(outBuffSize) {

}

void httpcon::init(std::shared_ptr<etEpoll> epollPtr, std::string home_page) {
    ep = epollPtr;
    HOME_PAGE = home_page;
}

// 接收新连接，返回 true 建立成功，否则返回 false
bool httpcon::startCon(int sockfd) {
    if(isClosed == false) return false;
    ep -> add(sockfd, EPOLLIN|EPOLLET);
    in.init();
    out.init();
    clntfd = sockfd;
    ++clntCount;
    isClosed = false;
    return true;
}

void httpcon::closeCon() {
    if(isClosed == true) return;
    if(ep -> del(clntfd)) {
        close(clntfd);
        --clntCount;
        // std::cout << "clntCount :" << clntCount << " close fd : " << clntfd << std::endl;
        clntfd = -1;
        isClosed = true;
    }
}

void httpcon::read() {
    // 非阻塞读
    while(true) {
        int tmp = in.readFromFd(clntfd);
        if(tmp == 0) break; // 读完
        else if(tmp == 2) {
            std::cerr << "httpcon::read() error 2" << std::endl;
            closeCon();
            break;
        }
        else if(tmp == 3) { // 过长，可以整个动态长度，或者清空无用缓冲，暂略
            std::cerr << "该请求输入缓冲区已满" << std::endl;
            closeCon();
            break;
        }
        else if(tmp == 4) { // 关闭
            closeCon();
            break;
        }
        // 1 继续
    }
    // in.coutall();
    process();
}

void httpcon::process() {
    request.init();
    httprequest::HTTP_CODE res = request.parse(in);
    // std::cout << "状态 " << res << std::endl;
    // std::cout << "方法 " << request.methon() << std::endl;
    // std::cout << "路径 " << request.path() << std::endl;
    // std::cout << "版本 " << request.version() << std::endl;
    // std::cout << "Host " << request.host() << std::endl;

    // 没有请求，等待消息
    if(res == httprequest::NO_REQUEST) {
        std::cout << "wtf ?? NO_REQUEST" << std::endl;
        return;
    }
    // 组装
    response.init(HOME_PAGE, request.methon(), request.path(), request.version());
    response.compose(out, httprequest::BAD_REQUEST == res);
    write();
}

void httpcon::write() {
    int errorReson;
    ssize_t len;
    while(true) {
        len = out.writetofd(clntfd, errorReson);
        if(len == 0) {
            closeCon();
            return;
        }
        if(len < 0) {
            if(errorReson == EAGAIN) {
                if(!ep -> add(clntfd, EPOLLIN|EPOLLET|EPOLLOUT)) {
                    std::cout << "注册失败" << std::endl;
                    closeCon();
                }
                return;
            }
            closeCon();
            return;
        }
    }
}

}
