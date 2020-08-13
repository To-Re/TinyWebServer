#include <sys/uio.h>
#include <iostream>

#include "buffer.h"

namespace wzy {

bool inbuffer::BeginLeftPtrMovetoR(int moveRight) {
    if(moveRight < 0 || leftPos+moveRight > rightPos) {
        std::cerr << "BeginLeftPtrMovetoR error()" << std::endl;
        return false;
    }
    leftPos += moveRight;
    return true;
}

const char* inbuffer::getfindPtr(const std::string & endString) {
    // 可以用 kmp 但是这给解析 http 用没必要，结束字符才两个长度
    int len = endString.size();
    if(len == 0) return zerobeginPtr() + rightPos;
    int i = leftPos;
    for(i = leftPos; i < rightPos; ++i) {
        if(i+len-1 >= rightPos) break;
        int j = 0;
        for(j = 0; j < len && endString[j] == mbuffer[i+j]; ++j) {}
        if(j == len) break;
    }
    return zerobeginPtr() + rightPos;
}

// 0 读完，1 没读完，2 读失败, 3 输入过大，并不大算自适应，4 客户端关闭
int inbuffer::readFromFd(int fd) {
    if(rightPos >= BUFFSIZE) return -2; // 过大
    ssize_t str_len = read(fd, zerobeginPtr() + rightPos, BUFFSIZE);
    if(str_len == 0) { // 关闭
        return 0;
    }
    else if(str_len < 0) {
        if(errno == EAGAIN) // 读完
            return 0;
        std::cerr << "readFromFd() error" << std::endl; //其他错误
        return -1;
    }
    rightPos += str_len;
    return 1; // 未读完
}

}