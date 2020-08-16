#include "httprequest.h"
#include "string.h"

namespace wzy {

void httprequest::init() {
    METHON = PATH = VERSION = "";
    parseState = CHECK_STATE_REQUESTLINE;
}

httprequest::HTTP_CODE httprequest::parse(inbuffer &buff) {
    if(buff.canRead() <= 0) {
        return BAD_REQUEST;
    }
    HTTP_CODE res;
    while(true) {
        std::string line = buff.getString("\r\n");
        switch(parseState) {
        case CHECK_STATE_REQUESTLINE:
            if(parse_request_line(line) == BAD_REQUEST) {
                return BAD_REQUEST;
            }
            break; 
        case CHECK_STATE_HEADER:
            if(parse_headers(line) == BAD_REQUEST) {
                return BAD_REQUEST;
            }
            break;
        case CHECK_STATE_CONTENT:
            // std::cout << "让我看看 CONTENT 都有啥：" << line << std::endl;
            res = parse_content(line);
            if(line.empty()) parseState = CHECK_STATE_END;
            if(res == BAD_REQUEST) return BAD_REQUEST;
        default:
            // std::cout << "解析结束" << std::endl;
            return GET_REQUEST;
            break;
        }
    }
    return BAD_REQUEST;
}

httprequest::HTTP_CODE httprequest::parse_request_line(std::string & line) {
    std::istringstream linecin(line);
    if(!(linecin >> METHON)) {
        return BAD_REQUEST;
    }
    if(METHON != "GET" && METHON != "POST") {
        return BAD_REQUEST;
    }
    if(!(linecin >> PATH)) {
        return BAD_REQUEST;
    }
    if(!(linecin >> VERSION)) {
        return BAD_REQUEST;
    }
    parseState = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

httprequest::HTTP_CODE httprequest::parse_headers(std::string & line) {
    std::istringstream linecin(line);
    std::string tmp;
    if(!(linecin >> tmp)) {
        parseState = CHECK_STATE_CONTENT;
        return NO_REQUEST;
    }
    if(tmp == "Host:") {
        if(!(linecin >> HOST)) {
            return BAD_REQUEST;
        }
    }
    return NO_REQUEST;
}

httprequest::HTTP_CODE httprequest::parse_content(std::string &) {
    return NO_REQUEST;
}

}

/*
GET / HTTP/1.1
Host: 192.168.1.102:10002
Connection: keep-alive
*/