#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <unistd.h>

namespace wzy {

// 可以改成迭代器，日后再说

class buffer{
public:
    buffer(int buffSize = 1000) : mbuffer(buffSize), leftPos(0), rightPos(0), BUFFSIZE(buffSize) {}
    virtual ~buffer() = default;
    void init() { leftPos = 0, rightPos = 0; }
    const char* const cLeftPtr() { return zerobeginPtr() + leftPos; }
    const char* const cRightPtr() { return zerobeginPtr() + rightPos; }
protected:
    std::vector<char> mbuffer;
    size_t leftPos = 0, rightPos = 0, BUFFSIZE = 0;
    char* zerobeginPtr() { return &mbuffer[0]; }
};

class inbuffer : public buffer {
public:
    inbuffer(int buffSize = 1000) : buffer(buffSize) {}
    ~inbuffer() = default;
    bool canRead() { return rightPos > leftPos; }
    bool BeginLeftPtrMovetoR(int);
    const char* getfindPtr(const std::string & endString = "\r\n");
    // 0 读完，1 没读完，2 读失败, 3 输入过大，并不大算自适应，4 客户端关闭
    int readFromFd(int);
};

class outbuffer : public buffer {
public:

};

}

#endif