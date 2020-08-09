#include "threadpool.h"

namespace wzy {

// 工作线程
void *threadPool::worker(void *arg) {
    threadPool *pool = static_cast<threadPool *>(arg);
    while(pool->isClosed == false) {
        (pool->tasks).runOneTask();
    }
    return NULL;
}

}