#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <iostream>
#include <sstream>
#include "buffer.h"

namespace wzy {

class httprequest {
public:
    enum CHECK_STATE {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT,
        CHECK_STATE_END
    };
    enum HTTP_CODE {
        NO_REQUEST = 0, GET_REQUEST, BAD_REQUEST, NO_RESOURCE,
        FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION
    };
    httprequest() = default;
    ~httprequest() = default;
    void init();
    HTTP_CODE parse(inbuffer &);
    const std::string &methon() const { return METHOD; }
    const std::string &path() const { return PATH; }
    const std::string &version() const { return VERSION; }
    const std::string &host() const { return HOST; }
private:
    HTTP_CODE parse_request_line(std::string &);
    HTTP_CODE parse_headers(std::string &);
    HTTP_CODE parse_content(std::string &);
    CHECK_STATE parseState = CHECK_STATE_REQUESTLINE;
    std::string METHOD, PATH, VERSION, HOST;
};

}

#endif