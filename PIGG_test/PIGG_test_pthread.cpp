#include "PIGG_test.h"
// #include "algorithm_util.h"
#include <memory>
#include <cstring>
#include <stdio.h>
#include <unistd.h>     // sleep(1),usleep() 微秒级
#include <sys/stat.h>      // stat
#include <fstream>       
#include <mutex>  
 

namespace PIGG_init_test{


void *run_pthread1(void *arg){
    for(int i = 0;i < 6;i++){
        printf("thread running %d \n",i);
        sleep(1);
    }
}

int p_test = 0;
void *run_pthread2(void *arg){
    char* name = (char*)arg;
    for(int i = 0;i < 6;i++){
        p_test++;
        printf("thread %s running %d %d\n",name,i,p_test);
        printf("%d\n",p_test);
        sleep(1);
    }
}

void PIGG_test::test_pthread(){
    pthread_t pid;
    int ret = pthread_create(&pid,NULL,run_pthread1,NULL);

    if(ret != 0){
        printf("create pthread error!\n");
    }

    for(int i = 0;i < 3;i++){
        
        p_test++;
        printf("mian pthread :%d \n",i);
        printf("%d\n",p_test);
        sleep(1);
    }   
    printf("main thread will exit when pthread is over\n");
    pthread_join(pid,NULL); // 挂起当前线程，等待指定线程运行完毕
    printf("mian pthread exit \n");

    printf("%d\n",p_test);

    // ret = pthread_create(&pid,NULL,run_pthread2,"sub pthread");
}


int p_test2 = 0;
// volatile int p_test2 = 0;     // 还是无法解决
void *run_pthread3(void* arg){
    p_test2++;
    printf("run_pthread2 %d\n",p_test2);
}

void PIGG_test::test_pthread2(){    // 出现多线程竞争
    int pth_num = 100;
    pthread_t pid[pth_num];
    for(int i = 0;i < pth_num;i++){
        pthread_create(&pid[i],NULL,run_pthread3,NULL);
    }
    
}

// std::vector<int> pth_vector;
int p_test3 = 0;
std::mutex mtx;
PIGG_locker tmp_lock;
void *run_pthread4(void* arg){
    tmp_lock.PIGG_lock();
    // mtx.lock();
    p_test3++;
    // mtx.unlock();
    tmp_lock.PIGG_unlock();
    // pth_vector.push_back(p_test3);
    printf("run_pthread2 %d\n",p_test3);
}

void PIGG_test::test_pthread3(){    // 出现多线程问题
    int pth_num = 100;
    pthread_t pid[pth_num];
    for(int i = 0;i < pth_num;i++){
        pthread_create(&pid[i],NULL,run_pthread4,NULL);
    }

    // PrintVector(pth_vector);
    
}

#pragma comment(lib, "pthreadVC2.lib")
pthread_cond_t test_cond;
pthread_mutex_t test_mutex;
int i = 0;
void *run_ptread5(void *arg){

    while(i < 100){
        // pthread_mutex_lock(&test_mutex);
        i++;
        if(i%5 == 0)
            pthread_cond_signal(&test_cond);
        else
            printf("我是线程  1  打印的数都非5的倍数:  %d \n",i);
        // pthread_mutex_unlock(&test_mutex);
        sleep(0.1);
    }
}


void *run_ptread6(void *arg){
    while(1){
        pthread_mutex_lock(&test_mutex);
        // 条件变量来传递变量
        pthread_cond_wait(&test_cond,&test_mutex);   //获得信号之前，会重新获得互斥锁
        printf("我是线程  2  打印5的倍数:  %d \n",i);
        pthread_mutex_unlock(&test_mutex);
        
    }
}


void PIGG_test::test_pthread4(){    // 出现多线程问题
    pthread_t pid1;
    pthread_t pid2;
    pthread_cond_init(&test_cond,NULL);
    pthread_mutex_init(&test_mutex,NULL);
    pthread_create(&pid1,NULL,run_ptread5,NULL);
    pthread_create(&pid2,NULL,run_ptread6,NULL);
    sleep(200000);
    
}


}