#include "taskQueue.h"

namespace wzy {

bool taskQueue::addOneTask(std::function<void()> func) {
    uniqueLocker(tasksMutex);
    // queue.size() 之前以为不是 O(1) 的
    if(tasks.size() >= maxTastQueueNum)
        return false;
    tasks.emplace(func);
    tasksSem.post();
    return true;
}

bool taskQueue::runOneTask() {
    std::function<void()> task;
    tasksSem.wait();
    {
        uniqueLocker(tasksMutex);
        if(tasks.empty()) return false;
        task = std::move(tasks.front());
        tasks.pop();
    }
    task();
    return true;
}

}