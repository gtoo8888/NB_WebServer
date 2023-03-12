#ifndef _TASK_QUEUE_
#define _TASK_QUEUE_

#include <iostream>
#include <queue>
#include <pthread.h>

using callback = void(*)(void* arg);

struct Task{
    Task(){
        function = nullptr;
        arg = nullptr;
    }
    Task(callback fun,void* arg){
        function = fun; // 没有添加这一句
        this->arg = arg;
    }
    callback function;
    void* arg;
};


// 任务队列
class TaskQueue{
public:
    TaskQueue();
    ~TaskQueue();

    void addTask(Task task);//添加任务
    Task tackTask();//取出一个任务
    inline int taskNumber(){//获取当前任务的个数
        return m_taskQ.size();
    }
private:
    pthread_mutex_t m_mutex;
    std::queue<Task> m_taskQ;
};




#endif