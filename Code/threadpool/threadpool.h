#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

#include <utility>
#include <vector>
#include <cstddef>
#include <iostream>
#include <initializer_list>

#include "config.h"
#include "taskQueue.h"

namespace wzy {

template <typename T>
class threadPool{
public:
    threadPool(
        size_t workNum = config::WORK_THREAD_NUM,
        size_t maxTasks = config::TASK_QUEUE_NUM
    );
    ~threadPool();
    // 添加任务
    bool addTask(const T & tmpTask, taskType type = READ);
    // bool addTask(const std::initializer_list<T> & tmpTasks, taskType type = READ);
    // bool addTask(std::initializer_list<T> &&);
private:
    // 工作线程数
    size_t workThreadNum;
    // 线程数组
    std::vector<pthread_t> workers;
    // 任务队列
    taskQueue<T> tasks;
    // 是否关闭
    bool isClosed = false;
    static void *worker(void *arg);
};

template <typename T>
threadPool<T>::threadPool(
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

template <typename T>
threadPool<T>::~threadPool() {
    isClosed = true;
    tasks.close();
    for(auto & v: workers) {
        pthread_join(v, NULL);
    }
}

// 添加任务
template <typename T>
bool threadPool<T>::addTask(const T &tmpTask, taskType type) {
    return tasks.addTask(tmpTask, type);
}

// template <typename T>
// bool threadPool<T>::addTask(const std::initializer_list<T> &tmpTasks, taskType type) {
//     return tasks.addTask(tmpTasks, type);
// }

// template <typename T>
// bool threadPool<T>::addTask(std::initializer_list<T> &&tmpTasks) {
//     return tasks.addTask(tmpTasks);
// }

// 工作线程
template <typename T>
void *threadPool<T>::worker(void *arg) {
    threadPool *pool = static_cast<threadPool *>(arg);
    while(pool->isClosed == false) {
        std::vector<std::pair<T, taskType> > tmpTasks = (pool->tasks).getTask(1);
        for(auto & task : tmpTasks) {
            if(task.second == READ) {
                std::cout << "READ" << std::endl;
                task.first();
            }
            else {
                std::cout << "WRITE" << std::endl;
                task.first();
            }
        }
    }
    return NULL;
}

}

#endif