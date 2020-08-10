#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/*
封装了，信号量、互斥锁、条件变量
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
    void wait() {
        sem_wait(&my_sem);
    }
    void post() {
        sem_post(&my_sem);
    }
private:
    sem_t my_sem;
};

// 互斥锁
class uniqueLocker {
public:
    // 拷贝后不同，需要传引用
    explicit uniqueLocker(pthread_mutex_t &mutex) : my_mutex(mutex) {
        pthread_mutex_lock(&my_mutex);
    }
    ~uniqueLocker() {
        pthread_mutex_unlock(&my_mutex);
    }
    uniqueLocker(const uniqueLocker &) = delete;
    uniqueLocker& operator= (const uniqueLocker&) = delete;
private:
    pthread_mutex_t& my_mutex;
};

// 条件变量
class cond {
public:
    cond() {
        if(pthread_cond_init(&my_cond, NULL)) {
            throw std::exception();
        }
    }
    ~cond() {
        pthread_cond_destroy(&my_cond);
    }
    void wait(pthread_mutex_t &m_mutex) {
        pthread_cond_wait(&my_cond, &m_mutex);
    }
    void signal() {
        pthread_cond_signal(&my_cond);
    }
    void broadcast() {
        pthread_cond_broadcast(&my_cond);
    }
private:
    pthread_cond_t my_cond;
};

}

#endif