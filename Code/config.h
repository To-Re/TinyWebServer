#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstddef>

namespace config {
    // 端口号
    const size_t PORT = 11451;

    // 工作线程
    const size_t WORK_THREAD_NUM = 4;

    // 任务队列大小
    const size_t TASK_QUEUE_NUM = 11451;

    // epoll 事件接收范围
    const size_t EPOLL_SIZE = 11451;

    // 最大任务数量及客户端文件描述符数量
    const size_t MAX_FD = 65536 - 10;

    // tcpSocket 请求等待队列长度
    const int MAX_BACKLOG = 20;

    // 支持绝对路径和相对路径
    const std::string HOME_PAGE = "page";
};

#endif