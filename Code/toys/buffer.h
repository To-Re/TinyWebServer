#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <fstream>

namespace wzy {

// 可以改成迭代器，日后再说

class buffer{
public:
    buffer(int buffSize = 1000) : leftPos(0), rightPos(0), BUFFSIZE(buffSize) {
        mbuffer.resize(buffSize, 0);
    }
    virtual ~buffer() = default;
    void init() { leftPos = 0, rightPos = 0; }
    void coutall() { std::cout << mbuffer << std::endl;}
    void coutPtr() { std::cout << "L : " << leftPos << " R : " << rightPos << std::endl; }
protected:
    std::string mbuffer;
    size_t leftPos = 0, rightPos = 0, BUFFSIZE = 0;
    char* zerobeginPtr() { return &mbuffer[0]; }
};

class inbuffer : public buffer {
public:
    inbuffer(int buffSize = 1000) : buffer(buffSize) {}
    ~inbuffer() = default;
    bool canRead() { return rightPos > leftPos; }
    std::string getString(const std::string & endString = "\r\n");
    // 0 读完，1 没读完，2 读失败, 3 输入过大，并不大算自适应，4 客户端关闭
    int readFromFd(int);
};

class outbuffer : public buffer {
public:
    outbuffer(int buffSize = 0) : buffer(buffSize) {}
    ~outbuffer() = default;
    void clear();
    bool add(const std::string &);
    int getFileLen(const std::string &);
    void addfile(const std::string &);
    ssize_t writetofd(int fd, int &);
};

}

#endif