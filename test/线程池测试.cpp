#include "locker.h"
#include "threadpool.h"
#include <unistd.h>
#include <iostream>
using namespace std;


void fk(int a, int b) {
    cout << a << " + " << b << " = " << a + b << endl;
}

int main() {
    cout << config::PORT << endl;
    threadPool a;
    a.addTask( []{cout << "fuck" << endl;} );
    a.addTask(bind(fk, 3, 6));
    sleep(2);
    return 0;
}
