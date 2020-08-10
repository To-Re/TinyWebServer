#include "locker.h"
#include "threadpool.h"
#include <unistd.h>
#include <iostream>
#include <functional>
using namespace std;


void fk(int a, int b) {
    cout << a << " + " << b << " = " << a + b << endl;
}

void md() {
    cout << "wtf" << endl;
}

int main() {
    cout << config::PORT << endl;
    wzy::threadPool<function<void()> > a;
    a.addTask( []{cout << "fuck" << endl;} );
    a.addTask({bind(fk, 4, 6), bind(fk, 4, 6)});
    a.addTask({bind(fk, 4, 6)});
    a.addTask({md});
    a.addTask(md);
    a.addTask(md);
    a.addTask(std::move(md));
    sleep(2);
    return 0;
}
