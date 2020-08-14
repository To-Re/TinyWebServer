#include "webserver.h"

using namespace std;

int main() {
    wzy::webserver server(10002);
    //监听
    server.listen();
    //运行
    server.start();
    return 0;
}
