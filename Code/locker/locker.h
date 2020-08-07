#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/*
封装了，信号量、互斥锁、条件变量，还未测试。拷贝控制成员可能有较大问题。
感觉应该使用智能指针，但本人能力有限，还是先学习其他人的一些方法
*/

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
class cond;
class locker {
public:
    friend class cond;
    locker() {
        if(pthread_mutex_init(&my_mutex, NULL)) {
            throw std::exception();
        }
    }
    ~locker() {
        pthread_mutex_destroy(&my_mutex);
    }
    locker(const locker &) = default;
    locker& operator= (const locker&) = default; // 有线程阻塞时，被赋值可能出现问题
    bool lock() {
        return pthread_mutex_lock(&my_mutex) == 0;
    }
    bool unlock() {
        return pthread_mutex_unlock(&my_mutex) == 0;
    }
private:
    pthread_mutex_t my_mutex;
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
    cond(const cond &) = default;
    cond& operator= (const cond &) = default; // 有线程阻塞时，被赋值可能出现问题
    bool wait(locker &my_lock) {
        return pthread_cond_wait(&my_cond, &my_lock.my_mutex) == 0;
    }
    bool signal() {
        return pthread_cond_signal(&my_cond) == 0;
    }
private:
    pthread_cond_t my_cond;
};

#endif