#include "etEpoll.h"
#include "tcpsocket.h"
#include "config.h"
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
    wzy::tcpSocket ser, clnt;
    ser.socket();
    ser.bind(config::PORT);
    ser.listen(100);
    ser.setNonBlocking();

    wzy::etEpoll ep;
    ep.create();
    ep.add(ser, EPOLLIN|EPOLLET);

    while(1) {
        int cnt = ep.wait(-1);
        if(cnt == -1) break;
        for(int i = 0; i < cnt; ++i) {
            if(ep.events[i].data.fd == ser) {
                while(ser.nonBlockingAccept(clnt)) {
                    clnt.setNonBlocking();
                    ep.add(clnt, EPOLLIN|EPOLLET);
                    cout << "connected client : " << clnt << endl;
                }
            }
            else {
                while(1) {
                    char buf[100];
                    int str_len = read(ep.events[i].data.fd, buf, sizeof(buf));
                    if(str_len == 0) { // 关闭
                        ep.del(ep.events[i].data.fd);
                        close(ep.events[i].data.fd);
                        printf("closed client : %d \n", ep.events[i].data.fd);
                        break;
                    }
                    else if(str_len < 0) {
                        if(errno == EAGAIN)
                            break;
                        cerr << "read error" << endl;
                        return 0;
                    }
                    write(ep.events[i].data.fd, buf, str_len);
                }
            }
        }
    }
    return 0;
}
/*
一个简单回声服务器及其测试
wzy@wzypc:~/TinyWebServer/test/build$ ./unit_test 
connected client : 7
connected client : 8
closed client : 8 
connected client : 8
closed client : 8 
closed client : 7 
^C
wzy@wzypc:~/TinyWebServer/test/build$ 

wzy@wzypc:~$ nc 127.0.0.1 11451
2
2
3
3
^C
wzy@wzypc:~$ nc 127.0.0.1 11451
^C
wzy@wzypc:~$ 

wzy@wzypc:~$ nc 127.0.0.1 11451
5
5
^C
wzy@wzypc:~$ 

*/