#ifndef PIGG_WEBSERVER_H
#define PIGG_WEBSERVER_H

#include "PIGG_log/PIGG_log.h"
#include <iostream>
#include <sys/epoll.h>
#include <errno.h>  //errno


#include <sys/socket.h> //socket(),bind(),listen()
#include <cstring>     //bzero()
#include <cassert>
#include <arpa/inet.h> //htons(),inet_pton()

#include "./PIGG_http/PIGG_http.h"

const int MAX_EVENT_NUMBER = 10000; //最大事件数


class PIGG_WebServer{
public:
    PIGG_WebServer();
    ~PIGG_WebServer();

    void init(int port, std::string user, std::string passWord, std::string databaseName,
                            bool close_log,bool log_queue);

    void log_write();
    void init_sql_pool();   // 初始化mysql的线程池
    void init_trig_mod(int trig_mode,int opt_LINGER);   // 初始化触发模式
    void event_listen();        // 开始监听
    void event_loop();          // 主函数开始循环

    void adjust_timer(int * timer); // 判断时间
    void deal_timer(int * timer,int sockfd);    // 处理时间
    bool dealclientdata();  // 处理客户端的数据
    bool dealwithsignal(bool &timeout,bool &stop_server);   // 处理信号
    void dealwithread(int sockfd);      // 处理读取
    void dealwithwrite(int sockfd);     // 处理写

private:
    // 基础设置
    int PIGG_port;
    int PIGG_close_log;         // 是否关闭日志
    int PIGG_log_queue;         // 是否要开启日志的阻塞队列

    PIGG_http_conn* PIGG_http_users;
    char* PIGG_root_path;


    // 数据库相关
    std::string PIGG_user;
    std::string PIGG_passWord;
    std::string PIGG_databaseName;
    int PIGG_sql_num;   // 数据库的链接数
    // connection_pool *PIGG_connPool;     // 数据库连接池

    // epoll_event相关
    epoll_event events[MAX_EVENT_NUMBER];

    int PIGG_listenfd;          // 监听描述符,socket返回的
    int PIGG_opt_LINGER;        // 优雅关闭连接，默认不使用
    int PIGG_trig_mode;         // 控制下面两个的触发模式
    int PIGG_listen_trig_mode;  // 连接触发模式
    int PIGG_conn_trig_mode;    // 监听触发模式

    int PIGG_epollfd;
    int PIGG_pipefd[2];

};


#endif 