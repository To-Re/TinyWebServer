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
    while(buff.canRead() && parseState != CHECK_STATE_END) {
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
            else return GET_REQUEST; // 只解析到 HOST，后面略
            break;
        case CHECK_STATE_CONTENT:
            if(parse_content(line) == BAD_REQUEST) {
                return BAD_REQUEST;
            }
        default:
            break;
        }
    }
    return NO_REQUEST;
}

httprequest::HTTP_CODE httprequest::parse_request_line(std::string & line) {
    std::istringstream linecin(line);
    if(!(linecin >> METHON)) {
        return BAD_REQUEST;
    }
    if(METHON != "GET") {
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
    if(!(linecin >> tmp) || tmp != "Host:") {
        return BAD_REQUEST;
    }
    if(!(linecin >> HOST)) {
        return BAD_REQUEST;
    }
    parseState = CHECK_STATE_CONTENT;
}

httprequest::HTTP_CODE httprequest::parse_content(std::string &) {
    return BAD_REQUEST;
}

}

/*
GET / HTTP/1.1
Host: 192.168.1.102:10002
Connection: keep-alive
*/