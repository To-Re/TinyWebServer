#include <bits/stdc++.h>
using namespace std;
#include "locker.h"
#include <pthread.h>
#include <unistd.h>

int num = 0;
locker a;
cond b[5];

// void *out(void *arg) {
//     int id = *static_cast<int*>(arg);
//     while(a.lock()) {
//         if(num > 10) {
//             a.unlock();
//             break;
//         }
//         b[id].wait(a);
//         cout << id << ": " << num++ << endl;
//         a.unlock();
//         // 不知道该怎么解决一个困惑
//         // 0线程如果唤醒1线程，这时0线程还没进入wait，4线程已经signal，然后0线程再也接收不到信号，0线程死锁
//         b[(id+1)%5].signal();
//     }
//     cout << id << " end" << endl;
//     return NULL;
// }

// 解决了刚刚的问题，感觉还是有问题，看别人使用条件变量前都会锁一下，条件变量、互斥量还得看书仔细学习下
// 暂略
void *out(void *arg) {
    int id = *static_cast<int*>(arg);
    while(b[id].wait(a)) {
        if(num > 10) {
            a.unlock();
            b[(id+1)%5].signal();
            break;
        }
        cout << id << ": " << num++ << endl;
        b[(id+1)%5].signal();
    }
    cout << id << " end" << endl;
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread_id[5];
    int wtf[5] = {0, 1, 2, 3, 4};
    for(int i = 0; i < 5; ++i){
        pthread_create(&thread_id[i], NULL, out, static_cast<void *>(&wtf[i]));
    }
    b[0].signal();
    for(int i = 0; i < 5; ++i){
        pthread_join(thread_id[i], NULL);
    }
    cout << "end" << endl;
    return 0;
}
