#include "ThreadPool.h"
#include <unistd.h>
ThreadPool::ThreadPool(int min,int max){
    // 实例化任务队列
    do{
        taskQ = new TaskQueue;
        if(taskQ == nullptr){
            std::cout << "malloc taskQ fail" << std::endl;
            break;
        }
        threadIDs = new pthread_t[max];
        if(threadIDs == nullptr){
            std::cout << "malloc threadIDs fail" << std::endl;
            break;
        }
        memset(threadIDs,0,sizeof(pthread_t) * max);
        minNum = min;
        maxNum = max;
        busyNum = 0;
        liveNum = min;
        exitNum = 0;
        if(pthread_mutex_init(&mutexPool,NULL) != 0 ||
            pthread_cond_init(&notEmpty,NULL) != 0){
            std::cout << "mutex or condition init fail... \n";
            break;
        }
        shutdown = false;
        pthread_create(&managerID,NULL,manager,this);   // 创建管理者线程
        for(int i = 0;i < max;i++){
            pthread_create(&threadIDs[i],NULL,worker,this); // 创建工作线程
        }
        return;
    }while(0);
    if(threadIDs) delete[] threadIDs;
    if(taskQ) delete[] taskQ;
}

ThreadPool::~ThreadPool(){
    shutdown = true;
    pthread_join(managerID,NULL);
    for(int i = 0;i < liveNum;i++){
        pthread_cond_signal(&notEmpty);
    }
    if(taskQ){
        delete taskQ;
    }
    if(threadIDs){
        delete[] threadIDs;
    }

    pthread_mutex_destroy(&mutexPool);
    pthread_cond_destroy(&notEmpty);

}


void *ThreadPool::worker(void* arg){
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    while(true){
        pthread_mutex_lock(&pool->mutexPool);
        // 当前任务队列是否为空
        while(pool->taskQ->taskNumber() == 0 && !pool->shutdown){
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);//阻塞工作线程
            if(pool->exitNum > 0){//判断是不是要销毁线程
                pool->exitNum--;
                if(pool->liveNum > pool->minNum){
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    pool->threadExit();
                }
            }
        }
        //判断线程池是否被关闭了
        if(pool->shutdown){
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        }
        Task task = pool->taskQ->tackTask();
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working...\n",pthread_self());
        
        task.function(task.arg);
        delete task.arg;
        task.arg = NULL;
        
        printf("thread %ld end working...\n",pthread_self());
        pthread_mutex_lock(&pool->mutexPool);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexPool);
    }
}

void *ThreadPool::manager(void* arg){
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    while(!pool->shutdown){
        sleep(3);

        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->taskQ->taskNumber();
        int liveNum = pool->liveNum;
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexPool);

        //添加线程
        //任务的个数>存活的线程个数 & &存活的线程数<最大线程数
        if(queueSize > liveNum && liveNum < pool->maxNum){
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for(int i = 0;i < pool->maxNum&& counter < NUMBER && pool->liveNum < pool->maxNum;i++){
                if(pool->threadIDs[i] == 0){
                    pthread_create(&pool->threadIDs[i],NULL,worker,pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }
        //销毁线程
        // 忙的线程*2<存活的线程数& &存活的线程>最小线程数
        if(busyNum*2 < liveNum && liveNum > pool->minNum){
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            //让工作的线程自杀
            for(int i = 0;i < NUMBER;i++){
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
}


void ThreadPool::threadExit(){
    pthread_t tid = pthread_self();
    for(int i = 0;i < maxNum;i++){
        if(threadIDs[i] == tid){
            threadIDs[i] = 0;
            printf("threadExit() call, %ld exit...\n",tid);
            // std::cout << "threadExit() call," << std::to_string(tid) << "exit...\n";
            break;
        }
    }
    pthread_exit(NULL);
}

// 由于taskQ已经有线程锁了，所以不需要再这个里面再加了
void ThreadPool::addTask(Task task){
    if(shutdown){
        return;
    }
    taskQ->addTask(task);
    pthread_cond_signal(&notEmpty);
}

int ThreadPool::getBusyNum(){
    pthread_mutex_lock(&mutexPool);
    int busyNum = this->busyNum;
    pthread_mutex_unlock(&mutexPool);
    return busyNum;
}

int ThreadPool::getAliveNum(){
    pthread_mutex_lock(&mutexPool);
    int aliveNum = this->liveNum;
    pthread_mutex_unlock(&mutexPool);
    return aliveNum;
}


