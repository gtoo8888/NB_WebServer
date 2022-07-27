#include "PIGG_log.h"
#include <pthread.h>   
#include <cstring> 
#include <sys/time.h>   // gettimeofday()用于多线程中获取时间
#include <stdarg.h>     // 可变参的头文件，va_list 

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
        PIGG_log_queue = new PIGG_block_queue<std::string>(max_queue_size);
        pthread_t tid;  // 声明了一个线程
        // flush_log_thread是一个回调函数,这里表示创建线程异步写日志
        pthread_create(&tid,NULL,flush_log_thread,NULL); // 创建一个线程
    }

    PIGG_close_log = close_log; // 将外部传过来的参数转化到内部
    PIGG_log_buf_size = log_buf_size;
    PIGG_buf = new char[PIGG_log_buf_size];
    memset(PIGG_buf, '\0' , PIGG_log_buf_size);
    PIGG_split_lines = split_lines;         // 日志最大行数

    time_t t = time(NULL);
    struct tm *sys_tm = localtime(&t);
    struct tm PIGG_tm = *sys_tm;

    const char *p = strrchr(file_name, '/');    // 从前到后查找后面字符串第一个出现的位置，如果找到返回后面所有
    char log_full_name[256] = {0};

    if (p == NULL){// snprintf,将后面的字符串，格式化输出到缓冲区
        snprintf(log_full_name, 255, "%d_%02d_%02d_%s",
        PIGG_tm.tm_year + 1990,PIGG_tm.tm_mon + 1, PIGG_tm.tm_mday,file_name);
    }else {
        strcpy(log_name, p + 1);    // 将后面字符复制到前面
        strncpy(dir_name, file_name, p - file_name + 1);    // 最多复制第三个参数数量到第一个
        snprintf(log_full_name, 255 ,"%s%d_%02d_%02d_%s",dir_name,
        PIGG_tm.tm_year + 1990,PIGG_tm.tm_mon + 1, PIGG_tm.tm_mday,log_name);
    }
    
    PIGG_today = PIGG_tm.tm_mday;

    PIGG_fp = fopen(log_full_name, "a");    // 用一个指定的参数打开文件，“a”追加的形式
    if(PIGG_fp == NULL){    // 如果为空就失败了
        return false;
    }
    return true;    // 一直打开着了？感觉有很大的性能问题
}

void PIGG_log::write_log(int level,const char * format, ...){   // 可变参函数
    struct timeval now = {0,0}; // 定义一个空的结构体用来接取时间
    gettimeofday(&now,NULL);    // 多线程获得时间
    time_t t = now.tv_sec;  
    struct tm *sys_tm = localtime(&t);   // tm 也是sys.h中的结构体
    struct tm PIGG_tm = *sys_tm;              // my_tm刚刚拿到的时间
    char s[16] = {0};
    switch (level){
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
        strcpy(s, "[info]:");
        break;
    }
    //写入一个日志，增加日志行数, 在PIGG_split_lines最大行数的限制下
    PIGG_count++;
    // 当日期不是今天和写的日志的行数到达最大限制的时候，需要进行分页
    // if (PIGG_today != my_tm.tm_mday || PIGG_count % PIGG_split_lines == 0){
    //     char mew_log[255] = {0};
    //     fflush(PIGG_fp);
    // }

    va_list valist; // 用接收可变参数的列表，用来代替...
    va_start(valist, format);   // 这个可变参数...，前的变量一般用于表示变量的数量
    std::string log_str;

    // n表示插入的这堆参数的长度
    int n = snprintf(PIGG_buf, 48 ,"%d-%02d-%02d %02d:%02d:%02d.%06ld %s",
    PIGG_tm.tm_year + 1990,PIGG_tm.tm_mon + 1, PIGG_tm.tm_mday,
    PIGG_tm.tm_hour,PIGG_tm.tm_min, PIGG_tm.tm_sec, now.tv_usec, s);
    // 追加valist可变参数的内容
    int m = vsnprintf(PIGG_buf + n, PIGG_log_buf_size - 1, format, valist);
    PIGG_buf[n + m] = '\n';
    PIGG_buf[n + m + 1] = '\0';
    log_str = PIGG_buf; // 要写的东西先存起来

    if(PIGG_is_async && !PIGG_log_queue->full()){
        PIGG_log_queue->push(log_str);  // 如果一次要写很多日志，那么就先放在阻塞队列里面，一个一个取出来再写
    }else {
        fputs(log_str.c_str(),PIGG_fp); // 写到后面的文件描述符里面
    }
    va_end(valist); // 清理为valist保留的内容    
}

