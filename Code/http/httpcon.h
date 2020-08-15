#ifndef HTTPCON_H
#define HTTPCON_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include <string>
#include <memory>

#include "etEpoll.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "buffer.h"

namespace wzy {

// 一个 HTTP 连接
class httpcon {
public:
    // 用户数量
    static int clntCount;
    httpcon(int inBuffSize = 1000, int outBuffSize = 0);
    ~httpcon() { if(!isClosed) closeCon(); }
    // 初始化 init
    void init(std::shared_ptr<etEpoll> epollPtr, std::string home_page);
    // 接收新连接，返回 true 建立成功，否则返回 false
    bool startCon(int fd);
    // 关闭连接
    void closeCon();
    // 处理请求
    void process();
    void read();
    void write();
private:
    inbuffer in;
    outbuffer out;
    std::shared_ptr<etEpoll> ep; // 与 webserver 共用 epoll
    std::string HOME_PAGE; // 根目录
    bool isClosed = true; // 连接状态
    int clntfd = -1; // 客户端 socket
    httprequest request; // 解析类
    httpresponse response;// 应答类
};

}

#endif