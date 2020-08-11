#include <unistd.h>
#include <iostream>
#include <functional>
#include "locker.h"
#include "threadpool.h"
#include "tcpsocket.h"
using namespace std;


void fk(int a, int b) {
    cout << a << " + " << b << " = " << a + b << endl;
}

void md() {
    cout << "wtf" << endl;
}

int main() {
    int str_len, BUF_SIZE = 100;
    char s[100] = "wdnmd\n";
    wzy::tcpSocket ser, clnt;
    ser.Socket();
    ser.Bind(config::PORT);
    ser.Listen();
    ser.Accept(clnt);
    write(clnt, s, sizeof(s));
    while((str_len = read(clnt, s, BUF_SIZE)) != 0)
        write(clnt, s, str_len);
    clnt.Close();
    ser.setNonBlocking();
    while(ser.nonBlockingAccept(clnt) == false) {
        cout << "###?" << endl;
        sleep(1);
    }
    write(clnt, "hello ==\n", sizeof("hello ==\n"));
    memset(s,0,sizeof(s));
    while((str_len = read(clnt, s, BUF_SIZE)) != 0) {
        printf("%s",s);
        memset(s,0,sizeof(s));
    }
    clnt.Close();
    return 0;
}
/*
测试阻塞 accept 和 非阻塞 accept
wzy@wzypc:~/TinyWebServer/test/build$ ./unit_test 
###?
###?
###?
###?
ewq
wzy@wzypc:~/TinyWebServer/test/build$ 

上面服务器，下面客户端

wzy@wzypc:~$ nc 127.0.0.1 11451
wdnmd
qwe
qwe
^C
wzy@wzypc:~$ nc 127.0.0.1 11451
hello ==
ewq
^C
wzy@wzypc:~$ 

*/