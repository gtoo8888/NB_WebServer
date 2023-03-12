// 线程同步机制包装类
// ===============
// 多线程同步，确保任一时刻只能有一个线程能进入关键代码段.
// > * 信号量
// > * 互斥锁
// > * 条件变量
#ifndef _PIGG_LOCK_H_
#define _PIGG_LOCK_H_

#include <pthread.h>
#include <semaphore.h>  // 信号量头文件
#include <exception>

// 信号量
// 在线程池中使用
class PIGG_sem {
public:
    PIGG_sem(){
        if(sem_init(&PIGG_sem_t, 0, 0) != 0){
            // throw（抛出异常）
            throw std::exception(); //标准异常类
        }
    }
    PIGG_sem(int num){
        if(sem_init(&PIGG_sem_t, 0 ,num) != 0){ //信号量初始化
            throw std::exception();
        }
    }
    ~PIGG_sem(){
        sem_destroy(&PIGG_sem_t);//信号量销毁
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

// 线程锁
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

// 用于多线程的条件变量
class cond{
public:
    cond(){
        if(pthread_cond_init(&PIGG_cond, NULL) != 0){
            throw std::exception();
        }
    }
    ~cond(){
        pthread_cond_destroy(&PIGG_cond);
    }
    
    bool wait(pthread_mutex_t *t_mutex){
        int ret = 0;
        ret = pthread_cond_wait(&PIGG_cond, t_mutex);
        return ret == 0;
    }
    // 在Linux中常用的时间结构有struct timespec 和struct timeval
    //pthread_cond_timedwait()函数阻塞住调用该函数的线程，等待由cond指定的条件被触发
    bool timewait(pthread_mutex_t *t_mutex,struct timespec time){
        int ret = 0;
        ret = pthread_cond_timedwait(&PIGG_cond, t_mutex, &time);
        return ret == 0;
    }
    // pthread_cond_signal(&cond1)的的作用是唤醒所有正在pthread_cond_wait(&cond1,&mutex1)的至少一个线程
    bool signal(){
        return pthread_cond_signal(&PIGG_cond) == 0;
    }
    // pthread_cond_broadcast(&cond1)的作用是唤醒所有正在pthread_cond_wait(&cond1,&mutex1)的线程。
    bool broadcast(){
        return pthread_cond_broadcast(&PIGG_cond) == 0;
    }
private:
    pthread_cond_t PIGG_cond;   // 条件变量
    //当锁住的共享变量发生改变时，可能需要通知相应的线程（因为可能该共享变量涉及到多个线程），这时就需要用到pthread_cond_t这种条件变量来精准的通知某个或几个线程， 让他们执行相应的操作
};

#endif