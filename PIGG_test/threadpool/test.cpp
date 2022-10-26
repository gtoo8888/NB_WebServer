#include "ThreadPool.h"
#include <unistd.h>

void taskFunc(void* arg){
    int num = *(int*)arg;
    printf("thread %ld is working, number = %d\n",pthread_self(),num);
    sleep(1);
}


// int main(void){
//     ThreadPool pool(3,3);
//     for(int i = 0;i < 10;i++){
//         // printf("%d\n",i);
//         int * num = new int(i+100);
//         pool.addTask(Task(taskFunc,num));
//     }
//     // sleep(5);
//     return 0;
// }