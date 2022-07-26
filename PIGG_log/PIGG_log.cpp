#include "PIGG_log.h"
#include <pthread.h>    
#include <sys/time.h>   //gettimeofday()用于多线程中获取时间

PIGG_log::PIGG_log(){
    PIGG_count = 0;
    PIGG_is_async = false;  // 初始化的时候，使用同步日志
}

PIGG_log::~PIGG_log(){
    if (PIGG_fp != NULL){
        fclose(PIGG_fp);
    }
}

void PIGG_log::flush(void){
    fflush(PIGG_fp);
}

bool PIGG_log::init(const char* file_name,int close_log,int log_buf_size,int split_lines,int max_queue_size){
    // 如果设置了最大队列长度，说明一定要用队列了
    // 设置为异步模式
    if (max_queue_size >= 1) {
        PIGG_is_async = true;   // 异步模式打开
        PIGG_log_queue = new PIGG_block_queue<std::string>(max_queue_size)
        pthread_t tid;  // 声明了一个线程
        // flush_log_thread是一个回调函数,这里表示创建线程异步写日志
        pthread_create(&tid,NULL,flush_log_thread,NULL) // 创建一个线程
    }
}

void PIGG_log::write_log(int level,const char * fotmat,...){
    struct timeval now = {0,0}; // 定义一个空的结构体用来接取时间
    gettimeofday(&now,NULL);    // 多线程获得时间
    time_t t = now.tv_sec;  
    struct tm *sys_tm = localtime(&t);                  // tm 也是sys.h中的结构体
    struct tm my_tm = *sys_tm;
    char s[16] = {0};
    switch (level)
    {
    case 0:
        strcpy(s, "[debug]:");
        break;
    case 1:
        strcpy(s, "[info]:");
        break;
    case 2:
        strcpy(s, "[warn]:");
        break;
    case 3:
        strcpy(s, "[erro]:");
        break;
    default:
        strcpy(s, "[info]:")
        break;
    }
}