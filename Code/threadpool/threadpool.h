#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

#include <queue>
#include <vector>
#include <exception>
#include <functional>
#include <cstddef>

#include "locker.h"
#include "config.h"

class threadPool{
public:
    threadPool(
        size_t workNum = config::WORK_THREAD_NUM,
        size_t maxTasks = config::TASK_QUEUE_NUM
    );
    ~threadPool();
    bool addTask(std::function<void()>); // 添加任务
private:
    // 工作线程数
    size_t workThreadNum;
    // 任务队列上限
    size_t maxTastQueueNum;
    // 任务数量
    size_t tasksNum = 0;
    // 线程数组
    std::vector<pthread_t> workers;
    // 任务队列
    std::queue<std::function<void()>> tasks;
    // 任务队列互斥锁
    locker tasksLocker;
    // 任务队列信号量
    sem tasksSem;
    // 是否关闭
    bool isClosed = false;
    static void *worker(void *arg);
};

inline threadPool::threadPool(
    size_t workNum, size_t maxTasks
) : workThreadNum(workNum), maxTastQueueNum(maxTasks), tasksNum(0), isClosed(false) {
    workers.resize(workThreadNum);
    for(size_t i = 0; i < workNum; ++i) {
        if(pthread_create(&workers[i], NULL, worker, this)) {
            workers.clear();
            throw std::exception();
        }
    }
}

inline threadPool::~threadPool() {
    tasksLocker.lock();
    isClosed = true;
    tasksLocker.unlock();
    // 没找到，唤醒全部阻塞信号量的 API，循环调用
    for(size_t i = 0; i < workThreadNum; ++i)
        tasksSem.post();
    for(auto & v: workers) {
        pthread_join(v, NULL);
    }
}

// 工作线程
void *threadPool::worker(void *arg) {
    threadPool *pool = static_cast<threadPool *>(arg);
    while(pool->isClosed == false) {
        pool->tasksSem.wait();
        pool->tasksLocker.lock();
        if(pool->tasks.empty() || pool->isClosed) {
            pool->tasksLocker.unlock();
            continue;
        }
        std::function<void()> task = std::move(pool->tasks.front());
        pool->tasks.pop();
        --(pool->tasksNum);
        pool->tasksLocker.unlock();
        task();
    }
    return NULL;
}

// 添加任务
inline bool threadPool::addTask(std::function<void()> func) {
    tasksLocker.lock();
    if(tasksNum >= maxTastQueueNum) {
        tasksLocker.unlock();
        return false;
    }
    // 推入任务
    tasks.emplace(func);
    ++tasksNum;
    tasksSem.post();
    tasksLocker.unlock();
    return true;
}

#endif