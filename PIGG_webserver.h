#ifndef PIGG_WEBSERVER_H
#define PIGG_WEBSERVER_H

#include "PIGG_log/PIGG_log.h"
#include <iostream>
#include <sys/epoll.h>


const int MAX_EVENT_NUMBER = 10000; //最大事件数


class PIGG_WebServer{
public:
    // PIGG_WebServer();
    // ~PIGG_WebServer();

    void init(int close_log,int log_write);

    void log_write();
    void init_sql_pool();   // 初始化mysql的线程池
    void init_trig_mod();   // 初始化触发模式

private:
    int PIGG_close_log;         // 是否关闭日志
    int PIGG_log_queue;         // 是否要开启日志的阻塞队列


    // epoll_event相关
    epoll_event events[MAX_EVENT_NUMBER];

    int PIGG_trig_mode;
    int PIGG_conn_trig_mode;    // 监听触发模式
    int PIGG_listen_trig_mode;  // 连接触发模式

};


#endif 