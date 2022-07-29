/*************************************************************
*循环数组实现的阻塞队列，m_back = (m_back + 1) % m_max_size;  
*线程安全，每个操作前都要先加互斥锁，操作完后，再解锁
**************************************************************/
#ifndef _PIGG_BLOCK_QUEUE_H_
#define _PIGG_BLOCK_QUEUE_H_

// 需要把不加锁的问题测试出来
template <class T>
class PIGG_block_queue{// 这是一个用来存放日志信息的阻塞队列
public:
    PIGG_block_queue(int max_size = 1000){
        if (max_size <= 0){
            exit(-1);
        }
        PIGG_max_size = max_size;
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

    bool full(){
;
    }

    bool empty(){
;
    }

    bool front(T &value){
;
    }

    bool back(T &value) {
;
    }

    int size() {
;
    }

    int max_size() {
;
    }


    bool push(const T &item) {
;
    }

    bool pop(T &item) {
;
    }

    bool pop(T &item, int ms_timeout) { // 增加超时处理
;
    }
private:
    // locker PIGG_mutex;      // 多线程的一些东西
    // cond PIGG_cond;         // 条件？

    T* PIGG_array;  
    int PIGG_size;
    int PIGG_max_size;
    int PIGG_front;
    int PIGG_back;
};


#endif