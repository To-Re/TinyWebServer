#include "buffer.h"

namespace wzy {

std::string inbuffer::getString(const std::string & endString) {
    size_t pos = mbuffer.find(endString, leftPos);
    size_t tmppos = leftPos;
    if(pos == mbuffer.npos) { // 没找到
        leftPos = rightPos;
        return std::string(zerobeginPtr() + tmppos, zerobeginPtr() + rightPos);
    }
    leftPos = pos+endString.size();
    return std::string(zerobeginPtr() + tmppos, zerobeginPtr() + pos);
}

// 0 读完，1 没读完，2 读失败, 3 输入过大，并不打算自适应，4 客户端关闭
int inbuffer::readFromFd(int fd) {
    if(rightPos >= BUFFSIZE) return 3; // 过大
    ssize_t str_len = read(fd, zerobeginPtr() + rightPos, BUFFSIZE);
    if(str_len == 0) { // 关闭
        return 4;
    }
    else if(str_len < 0) {
        if(errno == EAGAIN) // 读完
            return 0;
        std::cerr << "readFromFd() error" << std::endl; //其他错误
        return 2;
    }
    rightPos += str_len;
    return 1; // 未读完
}

}