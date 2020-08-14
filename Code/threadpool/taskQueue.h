#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <queue>
#include <utility>
#include <cstddef>
#include <initializer_list>
#include <utility>

#include "config.h"
#include "locker.h"

namespace wzy {

enum taskType {
    READ = 0, WRITE
};

template <typename T>
class taskQueue{
public:
    taskQueue(size_t maxTasks = config::TASK_QUEUE_NUM)
    : maxTastQueueNum(maxTasks), isClosed(false) {
        pthread_mutex_init(&tasksMutex, NULL);
    }
    ~taskQueue() { close(); };
    bool addTask(const T &, taskType type = READ);
    // bool addTask(const std::initializer_list<T> &, taskType);
    std::vector<std::pair<T, taskType> > getTask(size_t aid = 1); // 服了，模板默认形参要在声明这写
    void close() {
        isClosed = true;
        tasksCond.broadcast();
        pthread_mutex_destroy(&tasksMutex);
    }
private:
    // 任务队列上限
    size_t maxTastQueueNum;
    // 任务队列
    std::queue<std::pair<T, taskType> > tasks;
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
bool taskQueue<T>::addTask(const T & tmpTask, taskType type) {
    uniqueLocker nowLock(tasksMutex);
    if(tasks.size() >= maxTastQueueNum)
        return false;
    tasks.push(std::make_pair(tmpTask, type));
    tasksCond.signal();
    return true;
}

// template <typename T>
// bool taskQueue<T>::addTask(const std::initializer_list<T> & tmpTasks, taskType type) {
//     uniqueLocker nowLock(tasksMutex);
//     // 第一个判断避免数据变负，虽然正常情况不会变负
//     if(tasks.size() > maxTastQueueNum || maxTastQueueNum - tasks.size() < tmpTasks.size())
//         return false;
//     for(const auto &ftask : tmpTasks) {
//         tasks.emplace(ftask);
//     }
//     tasksCond.signal();
//     return true;
// }

// template <typename T>
// bool taskQueue<T>::addTask(std::initializer_list<T> && tmpTasks) {
//     uniqueLocker nowLock(tasksMutex);
//     // 第一个判断避免数据变负，虽然正常情况不会变负
//     if(tasks.size() > maxTastQueueNum || maxTastQueueNum - tasks.size() < tmpTasks.size())
//         return false;
//     for(auto &ftask : tmpTasks) {
//         tasks.emplace(ftask);
//     }
//     tasksCond.signal();
//     return true;
// }

template <typename T>
std::vector<std::pair<T, taskType> > taskQueue<T>::getTask(size_t aid) {
    std::vector<std::pair<T, taskType> > res;
    {
        uniqueLocker nowLock(tasksMutex);
        while(tasks.size() < aid && isClosed == false) {
            tasksCond.wait(tasksMutex);
        }
        for(size_t i = 0; i < aid; ++i) {
            if(tasks.empty() || isClosed)
                return res;
            res.emplace_back( std::move(tasks.front()) );
            tasks.pop();
        }
    }
    return res;
}

}

#endif