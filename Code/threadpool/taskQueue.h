#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <queue>
#include <utility>
#include <cstddef>
#include <initializer_list>

#include "config.h"
#include "locker.h"

namespace wzy {

template <typename T>
class taskQueue{
public:
    taskQueue(size_t maxTasks = config::TASK_QUEUE_NUM)
    : maxTastQueueNum(maxTasks), isClosed(false) {
        pthread_mutex_init(&tasksMutex, NULL);
    }
    ~taskQueue() { close(); };
    bool addTask(const T &);
    bool addTask(const std::initializer_list<T> &);
    bool addTask(std::initializer_list<T> &&);
    std::vector<T> getTask(size_t aid = 1); // 服了，模板默认形参要在声明这写
    void close() {
        isClosed = true;
        tasksCond.broadcast();
        pthread_mutex_destroy(&tasksMutex);
    }
private:
    // 任务队列上限
    size_t maxTastQueueNum;
    // 任务队列
    std::queue<T> tasks;
    // 任务队列互斥锁
    pthread_mutex_t tasksMutex;
    // 任务队列信号量
    // sem tasksSem;
    // 任务队列条件变量
    cond tasksCond;
    // 是否关闭
    bool isClosed = false;
};

template <typename T>
bool taskQueue<T>::addTask(const T & tmpTask) {
    uniqueLocker nowLock(tasksMutex);
    if(tasks.size() >= maxTastQueueNum)
        return false;
    tasks.push(tmpTask);
    tasksCond.signal();
    return true;
}

template <typename T>
bool taskQueue<T>::addTask(const std::initializer_list<T> & tmpTasks) {
    uniqueLocker nowLock(tasksMutex);
    size_t remainingSpace = maxTastQueueNum - tasks.size();
    size_t needSpace = tmpTasks.size();
    if(remainingSpace < needSpace)
        return false;
    for(const auto &ftask : tmpTasks) {
        tasks.push(ftask);
    }
    tasksCond.signal();
    return true;
}

template <typename T>
bool taskQueue<T>::addTask(std::initializer_list<T> && tmpTasks) {
    uniqueLocker nowLock(tasksMutex);
    size_t remainingSpace = maxTastQueueNum - tasks.size();
    size_t needSpace = tmpTasks.size();
    if(remainingSpace < needSpace)
        return false;
    for(auto &ftask : tmpTasks) {
        tasks.emplace(ftask);
    }
    tasksCond.signal();
    return true;
}

template <typename T>
std::vector<T> taskQueue<T>::getTask(size_t aid) {
    std::vector<T> res;
    {
        uniqueLocker nowLock(tasksMutex);
        while(tasks.size() < aid && isClosed == false) {
            tasksCond.wait(tasksMutex);
        }
        for(size_t i = 0; i < aid; ++i) {
            if(tasks.empty() || isClosed)
                return res;
            res.emplace_back(std::move(tasks.front()));
            tasks.pop();
        }
    }
    return res;
}

}

#endif