#ifndef PIGG_WEBSERVER_H
#define PIGG_WEBSERVER_H

#include <iostream>


class PIGG_WebServer{
public:
    PIGG_WebServer();
    ~PIGG_WebServer();

    void log_write();
    void sql_pool();

private:
    int m_close_log;
    int m_log_write;
};


#endif 