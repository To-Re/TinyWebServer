#ifndef CONFIG_H
#define CONFIG_H

#include <cstddef>

namespace config {
    // 端口号
    const size_t PORT = 11451;

    // 工作线程
    const size_t WORK_THREAD_NUM = 4;

    // 任务队列大小
    const size_t TASK_QUEUE_NUM = 114514;
};

#endif