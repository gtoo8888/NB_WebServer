/*************************************************************
*循环数组实现的阻塞队列，m_back = (m_back + 1) % m_max_size;  
*线程安全，每个操作前都要先加互斥锁，操作完后，再解锁
**************************************************************/
#ifndef _PIGG_BLOCK_QUEUE_H_
#define _PIGG_BLOCK_QUEUE_H_

#include <pthread.h>
#include <sys/time.h>
#include "../PIGG_lock/PIGG_lock.h"
// 用数组模拟队列的操作，提供push和pop接口
// 数组内部用循环队列实现
// 需要把不加锁的问题测试出来
// 操作成功会返回ture,失败返回false
template <class T>
class PIGG_block_queue{// 这是一个用来存放日志信息的阻塞队列
public:
    PIGG_block_queue(int max_size = 1000){
        if (max_size <= 0){
            exit(-1);
        }
        PIGG_max_size = max_size;
        PIGG_array = new T[PIGG_max_size];
        PIGG_size = 0;
        PIGG_front = -1;
        PIGG_back = -1;
    }
    ~PIGG_block_queue(){    // 还需要加锁
        if (PIGG_array != NULL)
            delete []PIGG_array;
    }

    // 需要加锁
    void clear(){   // 清理队列
        PIGG_size = 0;
        PIGG_front = -1;
        PIGG_back = -1;
    }

    bool full(){    // 判断队列是否已经满了
        if (PIGG_size >= PIGG_max_size){
            return true;
        }
        return false;
    }

    bool empty(){   // 判断队列是否为空
        if (PIGG_size == 0){
            return true;
        }
        return false;
    }

    bool front(T &value){   // 返回队首元素
        if (PIGG_size == 0){
            return false;
        }
        value = PIGG_array[PIGG_front];
        return true;
    }

    bool back(T &value) {   // 返回队尾元素
        if(PIGG_size == 0){
            return false;
        }
        value = PIGG_array[PIGG_back];
        return true;
    }

    int size() {    // 为什么这一步需要这样奇怪的加锁
        return PIGG_size;
    }

    int max_size() {
        return PIGG_max_size;
    }
    //往队列添加元素，需要将所有使用队列的线程先唤醒
    //当有元素push进队列,相当于生产者生产了一个元素
    //若当前没有线程等待条件变量,则唤醒无意义
    bool push(const T &item) {  // 插入完以后需要解锁？
        if(PIGG_size >= PIGG_max_size){
            return false;
        }
        PIGG_back = (PIGG_back + 1) % PIGG_max_size;
        PIGG_array[PIGG_back] = item;
        PIGG_size++;
        return true;
    }

    bool pop(T &item) { // 弹出的时候可以获取到弹出的项目
        while(PIGG_size <= 0){
            return false;
        }
        PIGG_front = (PIGG_front + 1) % PIGG_max_size;
        item = PIGG_array[PIGG_front];
        PIGG_size--;
        return true;
    }

    bool pop(T &item, int ms_timeout) { // 增加超时处理
        struct timespec time = {0, 0};  // ns精度
        struct timeval now = {0, 0};    // ms精度
    }
private:
    PIGG_locker PIGG_mutex;      // 多线程的一些东西
    cond PIGG_cond;         // 多线程条件变量

    T* PIGG_array;      
    int PIGG_size;      // 组
    int PIGG_max_size;
    int PIGG_front;
    int PIGG_back;
};


#endif