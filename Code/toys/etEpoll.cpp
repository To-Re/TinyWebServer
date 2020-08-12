#include "etEpoll.h"

namespace wzy {

void etEpoll::create() {
    epfd = epoll_create(EPOLL_SIZE);
    if(epfd < 0) error_handling("etEpoll create error");
}

bool etEpoll::add(const int & addfd, const uint32_t & event) {
    struct epoll_event eve;
    eve.events = event;
    eve.data.fd = addfd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, addfd, &eve)) {
        error_handling("etEpoll add error", false);
        return false;
    }
    return true;
}

bool etEpoll::del(const int & delfd) {
    if(epoll_ctl(epfd, EPOLL_CTL_DEL, delfd, NULL)) {
        error_handling("etEpoll del error", false);
        return false;
    }
    return true;
}

int etEpoll::wait(int timeOut) {
    int event_cnt = epoll_wait(epfd, events, EPOLL_SIZE, timeOut);
    if(event_cnt < 0) {
        error_handling("epoll wait error", false);
        return -1;
    }
    else if (event_cnt == 0) {
        return 0;
    }
    return event_cnt;
}

}
