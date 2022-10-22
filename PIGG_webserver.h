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
#include <signal.h> // SIGALRM,SIGTERM

#include "./PIGG_config/PIGG_config.h"
#include "./PIGG_http/PIGG_http.h"
#include "./PIGG_timer/PIGG_lst_timer.h"
#include "./PIGG_threadpool/PIGG_threadpool.h"
#include "./CGImysql/sql_connection_pool.h"


const int MAX_EVENT_NUMBER = 10; //最大事件数
const int TIMESLOT = 5;//最小超时单位


class PIGG_WebServer{
public:
    PIGG_WebServer();
    ~PIGG_WebServer();

    void init(PIGG_Config& temp_config);

    void log_write();
    void sql_pool();   // 初始化mysql的连接池子
    void thread_pool();   // 初始化线程池
    void init_trig_mod(int trig_mode,int opt_LINGER);   // 初始化触发模式
    void event_listen();        // 开始监听
    void event_loop();          // 主函数开始循环

    void adjust_timer(PIGG_util_timer* timer); // 判断时间
    void deal_timer(PIGG_util_timer* timer,int sockfd);    // 处理时间
    bool deal_client_data();  // 处理客户端的数据
    bool deal_with_signal(bool &timeout,bool &stop_server);   // 处理信号
    void deal_with_read(int sockfd);      // 处理读取
    void deal_with_write(int sockfd);     // 处理写

    //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
    void PIGG_timer(int connfd,struct sockaddr_in clinet_address);
public:
    // 基础设置
    int PIGG_port;
    int PIGG_close_log;         // 是否关闭日志
    int PIGG_log_queue;         // 是否要开启日志的阻塞队列
    int PIGG_log_record_max;         // 日志文件记录的最大条数
    int PIGG_block_queue_max_len;    // 最大阻塞队列长度
    int PIGG_block_queue_max_wait;  // 最多有多少个消息可以在队列中等待
    int PIGG_actor_model;       // 运行模式reactor

    PIGG_http_conn* PIGG_http_users;    // 表示整个http连接的类
    char* PIGG_root_path;


    // 数据库相关
    PIGG_connection_pool *PIGG_connPool;     // 数据库连接池
    std::string PIGG_user;
    std::string PIGG_password;
    std::string PIGG_databasename;
    int PIGG_sql_num;   // 数据库连接数
    

    //线程池相关
    PIGG_threadpool<PIGG_http_conn> *PIGG_pool;
    int PIGG_thread_num;

    // epoll_event相关
    epoll_event events[MAX_EVENT_NUMBER];

    int PIGG_listenfd;          // 监听描述符,socket返回的
    int PIGG_opt_LINGER;        // 优雅关闭连接，默认不使用
    int PIGG_trig_mode;         // 控制下面两个的触发模式
    int PIGG_listen_trig_mode;  // 连接触发模式
    int PIGG_conn_trig_mode;    // 监听触发模式

    int PIGG_epollfd;
    int PIGG_pipefd[2];

    // 定时器相关
    PIGG_client_data *PIGG_users_timer; // 定时器传递的数据类型
    PIGG_Utils PIGG_webserver_utils;

};


#endif 