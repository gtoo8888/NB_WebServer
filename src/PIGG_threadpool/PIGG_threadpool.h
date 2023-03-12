// 半同步/半反应堆线程池
// ===============
// 使用一个工作队列完全解除了主线程和工作线程的耦合关系：主线程往工作队列中插入任务，工作线程通过竞争来取得任务并执行它。
// > * 同步I/O模拟proactor模式
// > * 半同步/半反应堆
// > * 线程池
#ifndef PIGG_THREADPOOL_H
#define PIGG_THREADPOOL_H


#include "../CGImysql/sql_connection_pool.h"
#include "../PIGG_lock/PIGG_lock.h"
#include <pthread.h>
#include <list>


template<typename T>
class PIGG_threadpool{
public:
    // thread_number是线程池中线程的数量
    // max_requests是请求队列中最多允许的、等待处理的请求的数量
    // connPool是数据库连接池指针
    PIGG_threadpool(int actor_model,PIGG_connection_pool *coonPool,int thread_number = 8,int max_request = 10000);
    ~PIGG_threadpool();
    //像请求队列中插入任务请求
    bool append(T *request, int state);
    bool append_p(T *request);
private:
    /*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
    static void *worker(void *arg);
    void run();

private:
    int PIGG_thread_number;                     //线程池中的线程数
    int PIGG_max_requests;                      //请求队列中允许的最大请求数
    pthread_t *PIGG_threads;                    //描述线程池的数组，其大小为m_thread_number
    PIGG_locker PIGG_queue_locker;              //保护请求队列的互斥锁
    PIGG_sem PIGG_queue_stat;                   //是否有任务需要处理
    std::list<T *> PIGG_work_queue;             //请求队列
    PIGG_connection_pool *PIGG_connPool;        //数据库连接池
    int PIGG_actor_model;                       //模型切换
};

// int pthread_create (pthread_t *thread_tid,                 //返回新生成的线程的id
//                     const pthread_attr_t *attr,         //指向线程属性的指针,通常设置为NULL
//                    void * (*start_routine) (void *),   //处理线程函数的地址
//                     void *arg);                         //start_routine()中的参数
template<typename T>
PIGG_threadpool<T>::PIGG_threadpool(int actor_model,PIGG_connection_pool *coonPool,int thread_number,int max_request):
PIGG_actor_model(actor_model),PIGG_thread_number(thread_number),PIGG_max_requests(max_request),PIGG_threads(NULL),PIGG_connPool(coonPool){
    if(thread_number <= 0 || max_request <= 0)
        throw std::exception(); // 这个是为了下一句不要报错
    PIGG_threads = new pthread_t[PIGG_thread_number];//线程id初始化
    if(!PIGG_threads)
        throw std::exception();
    for(int i = 0; i < thread_number;i++){
        //循环创建线程，并将工作线程按要求进行运行
        if(pthread_create(PIGG_threads + i,NULL,worker,this) != 0){
            delete[] PIGG_threads;
            throw std::exception();
        }
        //将线程进行分离后，不用单独对工作线程进行回收
        if(pthread_detach(PIGG_threads[i])){ //从状态上实现线程分离，注意不是指该线程独自占
            delete[] PIGG_threads;
            throw std::exception();
        }
    }
}

template<typename T>
PIGG_threadpool<T>::~PIGG_threadpool(){
    // delete[] PIGG_threads;
}

template<typename T>
bool PIGG_threadpool<T>::append(T *request, int state){
    PIGG_queue_locker.PIGG_lock();  // 加锁
    if(PIGG_work_queue.size() >= PIGG_max_requests){
        PIGG_queue_locker.PIGG_unlock();
        return false;
    }
    request->PIGG_state = state;        // ???
    PIGG_work_queue.push_back(request);
    PIGG_queue_locker.PIGG_unlock();    // 解锁
    PIGG_queue_stat.post();
    return true;
}

template<typename T>
bool PIGG_threadpool<T>::append_p(T *request){
    PIGG_queue_locker.PIGG_lock();  // 加锁
    //根据硬件，预先设置请求队列的最大值
    if(PIGG_work_queue.size() >= PIGG_max_requests){
        PIGG_queue_locker.PIGG_unlock();
        return false;
    }
    //添加任务
    PIGG_work_queue.push_back(request);
    PIGG_queue_locker.PIGG_unlock();    // 解锁
    PIGG_queue_stat.post();//信号量提醒有任务要处理
    return true;
}

template<typename T>
void *PIGG_threadpool<T>::worker(void *arg){
    PIGG_threadpool *pool = (PIGG_threadpool *)arg;
    pool->run();
    return pool;
}

template<typename T>
void PIGG_threadpool<T>::run(){
    while(true){
        PIGG_queue_stat.wait();//信号量等待
        PIGG_queue_locker.PIGG_lock();//被唤醒后先加互斥锁
        if(PIGG_work_queue.empty()){    // 工作队列为空直接跳过
            PIGG_queue_locker.PIGG_unlock();
            continue;
        }
        //从请求队列中取出第一个任务
        //将任务从请求队列删除
        T* request = PIGG_work_queue.front();
        PIGG_work_queue.pop_front();
        PIGG_queue_locker.PIGG_unlock();
        if(!request)
            continue;
        if(PIGG_actor_model == 1){
            if(request->PIGG_state == 0){
                if(request->read_once()){
                    request->improv = 1;
                    PIGG_connection_RALL PIGG_mysqlcon(&request->mysql,PIGG_connPool);
                    request->process();//process(模板类中的方法,这里是http类)进行处理
                }else{
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }else{
                if(request->write()){
                    request->improv = 1;
                }else{
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
        }else{
            PIGG_connection_RALL PIGG_mysqlcon(&request->mysql,PIGG_connPool);
            request->process();
        }

    }
}





#endif /* PIGG_THREADPOOL_H*/