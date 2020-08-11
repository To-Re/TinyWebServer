#include "tcpsocket.h"

namespace wzy {

void tcpSocket::socket() {
    sockfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void tcpSocket::bind(int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if(::bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
        error_handling("bind() error");
}

void tcpSocket::listen(int backlog) {
    if(::listen(sockfd, backlog) == -1)
        error_handling("listen() error");
}

void tcpSocket::accept(tcpSocket &clnt_socket) {
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    clnt_socket.setsockfd( ::accept(sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_size) );
}

bool tcpSocket::nonBlockingAccept(tcpSocket &clnt_socket) {
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_fd = ::accept(sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_fd == -1) {
        if(errno == EAGAIN)
            return false;
        error_handling("nonBlockingAccept() error");
    }
    clnt_socket.setsockfd(clnt_fd);
    return true;
}

}