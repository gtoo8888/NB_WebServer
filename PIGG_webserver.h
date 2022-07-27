#ifndef PIGG_WEBSERVER_H
#define PIGG_WEBSERVER_H

#include "PIGG_log/PIGG_log.h"
#include <iostream>


class PIGG_WebServer{
public:
    // PIGG_WebServer();
    // ~PIGG_WebServer();

    void init(int close_log,int log_write);

    void log_write();
    void sql_pool();

private:
    int PIGG_close_log;         // 是否关闭日志
    int PIGG_log_queue;         // 是否要开启日志的阻塞队列
};


#endif 