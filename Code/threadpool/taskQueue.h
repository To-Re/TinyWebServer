#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <queue>
#include <cstddef>
#include <functional>

#include "config.h"
#include "locker.h"

namespace wzy {

class taskQueue {
public:
    taskQueue(size_t maxTasks = config::TASK_QUEUE_NUM) : maxTastQueueNum(maxTasks) {}
    ~taskQueue() = default;
    bool addOneTask(std::function<void()>);
    bool runOneTask();
    void post() {tasksSem.post();}
private:
    // 任务队列上限
    size_t maxTastQueueNum;
    // 任务队列
    std::queue<std::function<void()>> tasks;
    // 任务队列互斥锁
    pthread_mutex_t tasksMutex;
    // 任务队列信号量
    sem tasksSem;
};

}

#endif