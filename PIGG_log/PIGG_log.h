// 同步/异步日志系统
// ===============
// 同步/异步日志系统主要涉及了两个模块，一个是日志模块，一个是阻塞队列模块,其中加入阻塞队列模块主要是解决异步写入日志做准备.
// > * 自定义阻塞队列
// > * 单例模式创建日志
// > * 同步日志
// > * 异步日志
// > * 实现按天、超行分类
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
        static PIGG_log instance;       // 单例模式，创建了一个自己
        return &instance;               // 返回了指向自己的指针
    }

    static void *flush_log_thread(void *args){
        // PIGG_log::get_instance()->async_write_log();
    }
    // 强制刷新缓冲区
    void flush(void);   
    // 输出文件名,是否开启日志，日志缓冲区大小，分割线，最大队列长度
    bool init(const char* file_name,int close_log,int log_buf_size,int split_lines,int max_queue_size);
    // 输入日志等级、日志形式
    void write_log(int level,const char *format, ...);  

private:
    PIGG_log();
    virtual ~PIGG_log(); // 析构函数需要为虚函数
    // 但是没有继承，为什么需要虚函数？
    void *async_write_log(){                // 异步写日志,这是一个回调函数
        std::string single_log;  
        while(PIGG_log_queue->pop(single_log)){     //从阻塞队列中取出一个日志string，写入文件
            // PIGG_mutex.lock();                      //读写日志的时候需要加锁
            fputs(single_log.c_str(),PIGG_fp);          // 把字符串写到指定的流中，也就是把前面的字符串写到后面的FILE*这个类型的变量里面
            // PIGG_mutex.unlock();
        }
    }

private:
    char dir_name[128];     //路径名
    char log_name[128];     //log文件名
    int PIGG_split_lines;      //日志最大行数
    int PIGG_log_buf_size;     //日志缓冲区大小
    long long PIGG_count;      //日志行数记录
    int PIGG_today;            //因为按天分类,记录当前时间是那一天
    FILE *PIGG_fp;             //打开log的文件描述符
    char *PIGG_buf;
    bool PIGG_is_async;        //是否同步标志位 
    int PIGG_close_log;        //关闭日志
    PIGG_block_queue<std::string> *PIGG_log_queue;  //存放日志信息的阻塞队列
    // RAII的方式？不过没有析构

    // locker PIGG_mutex;         // 自己封装的锁
};

// 创建一个单例，调整日志等级，确定输入形式，传递参数
// ##__VA_ARGS__ , 表示可变参数 ,这个define一定得和主函数写在同一行，不然##__VA_ARGS__会报错
#define LOG_DEBUG(format, ...) if(true){PIGG_log::get_instance()->write_log(0, format, ##__VA_ARGS__);PIGG_log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(true){PIGG_log::get_instance()->write_log(1, format, ##__VA_ARGS__);PIGG_log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(true){PIGG_log::get_instance()->write_log(2, format, ##__VA_ARGS__);PIGG_log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(true){PIGG_log::get_instance()->write_log(3, format, ##__VA_ARGS__);PIGG_log::get_instance()->flush();}

// #define LOG_INFO(format, ...) if(0 == PIGG_close_log){
//     PIGG_log::get_instance()->write_log(1, format, ##__VA_ARGS__);  
//     PIGG_log::get_instance()->flush();
// }


#endif