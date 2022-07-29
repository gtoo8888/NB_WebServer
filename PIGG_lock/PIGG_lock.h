#ifndef _PIGG_LOCK_H_
#define _PIGG_LOCK_H_

#include <pthread.h>
#include <semaphore.h>  // 信号量头文件
#include <exception>

class PIGG_sem {
public:
    PIGG_sem(){
        if(sem_init(&PIGG_sem_t, 0, 0) != 0){
            // throw（抛出异常）
            throw std::exception(); //标准异常类
        }
    }
    PIGG_sem(int num){
        if(sem_init(&PIGG_sem_t, 0 ,num) != 0){
            throw std::exception();
        }
    }
    ~PIGG_sem(){
        sem_destroy(&PIGG_sem_t);
    }
    bool wait(){
        return sem_wait(&PIGG_sem_t) == 0;
    }
    bool post(){
        return sem_post(&PIGG_sem_t) == 0;
    }
private:
    sem_t PIGG_sem_t;
};
class PIGG_locker{
public:
    PIGG_locker(){  // 构造函数的时候创建一个线程
        if(pthread_mutex_init(&PIGG_mutex,NULL) != 0){
            throw std::exception();
        }
    }
    ~PIGG_locker(){ // 析构函数销毁锁
        pthread_mutex_destroy(&PIGG_mutex);
    }
    bool PIGG_lock(){   // 上锁
        return pthread_mutex_lock(&PIGG_mutex) == 0;
    }
    bool PIGG_unlock(){ // 解锁
        return pthread_mutex_unlock(&PIGG_mutex) == 0;
    }
    pthread_mutex_t *get(){
        return &PIGG_mutex;
    }
private:
    pthread_mutex_t PIGG_mutex; // 线程互斥量
};

#endif