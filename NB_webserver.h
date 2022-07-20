#ifndef NB_WEBSERVER_H
#define NB_WEBSERVER_H

#include <iostream>


class NB_WebServer{
public:
    NB_WebServer();
    ~NB_WebServer();

    void log_write();
    void sql_pool();

private:
    int m_close_log;
    int m_log_write;
};


#endif 