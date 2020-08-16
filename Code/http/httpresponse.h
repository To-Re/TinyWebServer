#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>
#include "buffer.h"

namespace wzy {

class httpresponse {
public:
    httpresponse() = default;
    ~httpresponse() = default;
    void init(const std::string & home, const std::string & methon, const std::string & path, const std::string & version) {
        HOME = home;
        METHON = methon;
        PATH = path;
        VERSION =version;
    }
    // 第一个参数传入 输出缓冲区
    // 第二个参数传入 true 表示语法错误返回 400
    void compose(outbuffer &, bool);
private:
    // 检查文件
    void check(int &, outbuffer &);
    static std::map<int, std::string> state;
    static std::map<int, std::string> merrorfile;
    static std::map<std::string, std::string> fileType;
    std::string HOME, METHON, PATH, VERSION;
};

}

#endif