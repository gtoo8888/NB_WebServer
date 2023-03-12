#ifndef PIGG_CONFIG_H
#define PIGG_CONFIG_H

#include <iostream>


class PIGG_Config{
public:
    PIGG_Config();
    ~PIGG_Config(){};

    void parse_arg(int arg,char *argv[]);
    bool get_json_config();

    std::string user;
    std::string passwd;
    std::string databasename;
    int port;               //端口号
    bool log_write;           //日志写入方式
    bool trig_mode;           //触发组合模式
    bool listen_trig_mode;     //listenfd触发模式 
    bool conn_trig_mode;       //connfd触发模式
    bool opt_linger;         //优雅关闭链接
    unsigned int sql_num;             //数据库连接池数量
    unsigned int thread_num;             //线程池内的线程数量
    bool close_log;                         //是否关闭日志  
    unsigned int log_record_max;         // 日志文件记录的最大条数
    unsigned int block_queue_max_len;    // 最大阻塞队列长度
    unsigned int block_queue_max_wait;  // 最多有多少个消息可以在队列中等待
    bool actor_model;        //并发模型选择  
    bool log_queue;         //日志队列
    std::string html_root;
};


#endif
