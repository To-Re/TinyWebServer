#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

#include <vector>
#include <cstddef>
#include <exception>
#include <functional>

#include "config.h"
#include "taskQueue.h"

namespace wzy {

class threadPool{
public:
    threadPool(
        size_t workNum = config::WORK_THREAD_NUM,
        size_t maxTasks = config::TASK_QUEUE_NUM
    );
    ~threadPool();
    // 添加任务
    bool addTask(std::function<void()> func) {
        return tasks.addOneTask(func);
    }
private:
    // 工作线程数
    size_t workThreadNum;
    // 线程数组
    std::vector<pthread_t> workers;
    // 任务队列
    taskQueue tasks;
    // 是否关闭
    bool isClosed = false;
    static void *worker(void *arg);
};

inline threadPool::threadPool(
    size_t workNum, size_t maxTasks
) : workThreadNum(workNum), tasks(maxTasks), isClosed(false) {
    workers.resize(workThreadNum);
    for(size_t i = 0; i < workNum; ++i) {
        if(pthread_create(&workers[i], NULL, worker, this)) {
            workers.clear();
            throw std::exception();
        }
    }
}

inline threadPool::~threadPool() {
    isClosed = true;
    // 没找到，唤醒全部阻塞信号量的 API，循环调用
    for(size_t i = 0; i < workThreadNum; ++i)
        tasks.post();
    for(auto & v: workers) {
        pthread_join(v, NULL);
    }
}

}

#endif