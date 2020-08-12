#include "webserver.h"

// 之后研究下单例模式
int main(int argc, char *argv[]) {
    wzy::webserver server;
    //监听
    server.listen();
    //运行
    server.start();
    return 0;
}