#ifndef PIGG_LOG_H
#define PIGG_LOG_H


#include <stdio.h>
#include <iostream>
#include "PIGG_block_queue.h"
//instance 实例
//async 异步
//split 分割
class PIGG_log{   // 这是一个单例模式，需要将构造函数私有化
public:
    static PIGG_log *get_instance(){  // 单例模式，只能通过接口来创建自己
        static PIGG_log instance; // 创建了一个
        return &instance;
    }

    static void *flush_log_thread(void *args){
        // PIGG_log::get_instance()->async_write_log();
    }
    void log_write();
    void sql_pool();

private:
    PIGG_log();
    virtual ~PIGG_log(); // 析构函数需要为虚函数
    // 但是没有继承，为什么需要虚函数？
    // void *async_write_log(){
    //     std::string single_log;  
    //     while(PIGG_log_queue->pop(single_log)){     //从阻塞队列中取出一个日志string，写入文件
    //         PIGG_mutex.lock();                      //读写日志的时候需要加锁
    //         fputs(single_log.c_str(),PIGG_fp);
    //         PIGG_mutex.unlock();
    //     }
    // }

private:
    char dir_name[128];     //路径名
    char log_name[128];     //log文件名
    int PIGG_split_lines;      //日志最大行数
    int PIGG_log_buf_size;     //日志缓冲区大小
    long long PIGG_count;      //日志行数记录
    int PIGG_today;            //因为按天分类,记录当前时间是那一天
    FILE *PIGG_fp;             //打开log的文件指针
    char *PIGG_buf;
    bool PIGG_is_async;        //是否同步标志位 
    int PIGG_close_log;        //关闭日志
    PIGG_block_queue<std::string> *PIGG_log_queue;  //存放日志信息的阻塞队列

    // locker PIGG_mutex;         // 自己封装的锁
};


#endif