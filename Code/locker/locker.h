#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/*
封装了，信号量、互斥锁
条件变量不封装了，感觉直接使用比较方便，还可以避免虚假唤醒（菜到不知道如何封装）
感觉应该使用智能指针，但本人能力有限，还是先学习其他人的一些方法
*/

namespace wzy {

// 信号量
class sem {
public:
    sem(int value = 0) {
        if(sem_init(&my_sem, 0, value)) {
            throw std::exception();
        }
    }
    ~sem() {
        sem_destroy(&my_sem);
    };
    sem(const sem &) = default;
    sem& operator= (const sem&) = default; // 有线程阻塞时，被赋值可能出现问题
    bool wait() {
        return sem_wait(&my_sem) == 0;
    }
    bool post() {
        return sem_post(&my_sem) == 0;
    }
private:
    sem_t my_sem;
};

// 互斥锁
class uniqueLocker {
public:
    uniqueLocker() {
        if(pthread_mutex_init(&my_mutex, NULL)) {
            throw std::exception();
        }
        pthread_mutex_lock(&my_mutex);
    }
    explicit uniqueLocker(pthread_mutex_t &mutex) : my_mutex(mutex) {
        if(pthread_mutex_init(&my_mutex, NULL)) {
            throw std::exception();
        }
        pthread_mutex_lock(&my_mutex);
    }
    ~uniqueLocker() {
        pthread_mutex_unlock(&my_mutex);
        pthread_mutex_destroy(&my_mutex);
    }
    uniqueLocker(const uniqueLocker &) = delete;
    uniqueLocker& operator= (const uniqueLocker&) = delete;
private:
    pthread_mutex_t my_mutex;
};

}

#endif