#include "httpresponse.h"

namespace wzy {

std::map<int, std::string> httpresponse::state = {
    {200, "OK"},
    {400, "Bad Request"},
    {403, "Forbidden"},
    {404, "Not Found"}
};

std::map<int, std::string> httpresponse::merrorfile = {
    {400, "400.html"},
    {403, "403.html"},
    {404, "404.html"},
};

void httpresponse::compose(outbuffer &out, bool is_400) {
    // std::cout << "compose : " << std::endl;
    // std::cout << HOME << std::endl;
    // std::cout << is_400 << std::endl;
    // std::cout << METHON << std::endl;
    // std::cout << PATH << std::endl;
    // std::cout << VERSION << std::endl;
    out.clear();
    int sta = 200;
    if(is_400) sta = 400;
    // 检查文件
    check(sta, out);
    // 状态行
    out.add(VERSION + " " + std::to_string(sta) + " " + state[sta] + "\r\n");
    // 响应头部
    // 长度+类型
    out.add("Content-length:" + std::to_string(out.getFileLen(PATH)) + "\r\n");
    out.add(std::string("Content-type:text/html") + "\r\n");
    out.add("\n");
    // 传输 PATH 对应文件
    out.addfile(HOME+PATH);
    // out.coutall();
}

void httpresponse::check(int &sta, outbuffer &out) {
    if(sta == 400) {
        sta = 400;
        PATH = merrorfile[sta];
        return;
    }
    if(PATH == "/" || PATH.empty()) PATH = "index.html";
    int len = out.getFileLen(HOME+PATH);
    if(len == -1) {
        sta = 404;
        PATH = merrorfile[sta];
        return;
    }
    sta = 200;
}

}