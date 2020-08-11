#include "tcpsocket.h"

namespace wzy {

bool tcpSocket::Socket() {
    sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    return sockfd != -1;
}

bool tcpSocket::Bind(int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    return bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) != -1;
}

bool tcpSocket::Listen(int backlog) {
    return listen(sockfd, backlog) != -1;
}

bool tcpSocket::Accept(tcpSocket &clnt_socket) {
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_fd = accept(sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_fd == -1) return false;
    clnt_socket.setsockfd(clnt_fd);
    return true;
}

}