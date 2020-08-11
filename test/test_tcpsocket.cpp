#include <unistd.h>
#include <iostream>
#include <functional>
#include "locker.h"
#include "threadpool.h"
#include "tcpsocket.h"
using namespace std;

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
    ser.Close();
    return 0;
}
/* 勉强算个测试 tcpsocket.h，应该能用，希望没有事
wzy@wzypc:~$ nc 127.0.0.1 11451
wdnmd
fuck
fuck
^C
wzy@wzypc:~$ 
*/