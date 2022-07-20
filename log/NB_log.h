#ifndef NB_LOG_H
#define NB_LOG_H


#include <stdio.h>
#include <iostream>
//instance 实例
//async 异步
//split 分割
class NB_log{   // 这是一个单例模式，需要将构造函数私有化
public:
    static NB_log *get_instance(){  // 单例模式，只能通过接口来创建自己
        static NB_log instance; // 创建了一个
        return &instance;
    }

    static void *flush_log_thread(void *args){
        NB_log::get_instance()->async_write_log();
    }
    void log_write();
    void sql_pool();

private:
    NB_log();
    virtual ~NB_log(); // 析构函数需要为虚函数
    // 但是没有继承，为什么需要虚函数？
    // void *async_write_log(){
    //     std::string single_log;  
    //     while(m_log_queue->pop(single_log)){     //从阻塞队列中取出一个日志string，写入文件
    //         m_mutex.lock();                      //读写日志的时候需要加锁
    //         fputs(single_log.c_str(),m_fp);
    //         m_mutex.unlock();
    //     }
    // }

private:
    char dir_name[128];     //路径名
    char log_name[128];     //log文件名
    int m_split_lines;      //日志最大行数
    int m_log_buf_size;     //日志缓冲区大小
    long long m_count;      //日志行数记录
    int m_today;            //因为按天分类,记录当前时间是那一天
    FILE *m_fp;             //打开log的文件指针
    char *m_buf;
    bool m_is_async;        //是否同步标志位 
    int m_close_log;        //关闭日志

    // block_queuq<string> * m_log_queue;       // 阻塞队列
    // locker m_mutex;         // 自己封装的锁
};


#endif