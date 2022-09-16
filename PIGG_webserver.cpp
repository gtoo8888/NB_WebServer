#include "PIGG_webserver.h"


// PIGG_WebServer::PIGG_WebServer(){

// }

// PIGG_WebServer::~PIGG_WebServer(){

// }

void PIGG_WebServer::init(int port, std::string user, std::string passWord, std::string databaseName,
                            bool close_log,bool log_queue){
    // 基础配置
    PIGG_port = port;
    PIGG_user = user;
    PIGG_passWord = passWord;
    PIGG_databaseName = databaseName;

    // 日志相关
    PIGG_close_log = close_log; // 是否关闭日志
    PIGG_log_queue = log_queue; // 开启日志队列
}

void PIGG_WebServer::log_write(){
    if(PIGG_log_queue == true){   // 是否写日志，开启就是写
        // 缓冲区长度2000, 一个日志文件记录的最大条数800000, 最大阻塞队列长度800，最多有800个消息可以在队列中等待
        PIGG_log::get_instance()->init("./ServerLog",PIGG_close_log, 2000, 800000, 800);
    }else{
        // 关闭了阻塞队列
        PIGG_log::get_instance()->init("./ServerLog",PIGG_close_log, 2000, 800000, 0);
    }
}
void PIGG_WebServer::init_sql_pool(){

}

void PIGG_WebServer::init_trig_mod(int trig_mode,int opt_LINGER){
    PIGG_trig_mode = trig_mode;         // 触发模式
    PIGG_opt_LINGER = opt_LINGER;       // 优雅关闭
    if (PIGG_trig_mode == 0) {          // LT+LT
        PIGG_listen_trig_mode = 0;
        PIGG_conn_trig_mode = 0;
    }else if (PIGG_trig_mode == 1) {    // LT+ET
        PIGG_listen_trig_mode = 0;
        PIGG_conn_trig_mode = 1;
    }else if (PIGG_trig_mode == 2) {    //ET+LT
        PIGG_listen_trig_mode = 1;
        PIGG_conn_trig_mode = 0;
    }else if (PIGG_trig_mode == 3) {    //ET+ET
        PIGG_listen_trig_mode = 1;
        PIGG_conn_trig_mode = 1;
    }
}

// 开始socket监听
// 很多时钟的内容没有添加
void PIGG_WebServer::event_listen(){
    //网络编程基础步骤
    PIGG_listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(PIGG_listenfd >= 0); // 如果小于0就报错

    // 是否开启优雅关闭连接，默认不开启
    if(PIGG_opt_LINGER == 0){
        struct linger tmp = {0, 1};
        // 获取或者设置与某个套接字关联的选项
        setsockopt(PIGG_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }else if(PIGG_opt_LINGER == 1){
        struct linger tmp = {1, 1};
        setsockopt(PIGG_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PIGG_port);    //将一个无符号短整型数值转换为网络字节序，即大端模式(big-endian)　
    address.sin_addr.s_addr = htonl(INADDR_ANY);   // 将主机的无符号长整形数转换成网络字节顺序

    int flag = 1;
    setsockopt(PIGG_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(PIGG_listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(PIGG_listenfd, 5);
    assert(ret >= 0);

    //epoll创建内核事件表
    epoll_event events[MAX_EVENT_NUMBER];
    PIGG_epollfd = epoll_create(5);
    assert(PIGG_epollfd != -1);

    // http_conn::PIGG_epollfd = PIGG_epollfd;
    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, PIGG_pipefd);
    assert(ret != -1);

    // 工具类,信号和描述符基础操作
    // 没有实现
}

// 主程序循环，不断接收请求
void PIGG_WebServer::event_loop(){
    bool timeout = false;
    bool stop_server = false;

    while(!stop_server){
        int number = epoll_wait(PIGG_epollfd, events, MAX_EVENT_NUMBER, -1);
        if(number < 0 && errno != EINTR){
            LOG_ERROR("%s", "epoll failure");
            break;
        }

        for(int i = 0;i < number; i++){
            int sockfd = events[i].data.fd;
            // 要处理新到的客户端请求
            if (sockfd == PIGG_listenfd){   //如果是老的连接，就跳过了
                // bool flag = delclinetdata();
                // if (flag == false)
                //     continue;
            }else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                //服务器端关闭连接，移除对应的定时器
                // deal_timer(timer,sockfd);
            }else if ((events[i].events & EPOLLIN) && ( sockfd == PIGG_pipefd[0])) {
                bool flag = dealwithsignal(timeout, stop_server);   // 处理信号
                if(flag == false){
                    LOG_ERROR("%s", "dealclientdata failure");
                }
            }else if (events[i].events & EPOLLIN) {     // 处理写数据
                dealwithread(sockfd);
            }else if (events[i].events & EPOLLOUT) {    // 处理读数据
                dealwithwrite(sockfd);
            }
        }
        if(timeout) {
            LOG_INFO("%s", "time tick");
            timeout = false;
        }
    }
}

// 判断时间
void PIGG_WebServer::adjust_timer(int * timer) {

}

// 处理时间
void PIGG_WebServer::deal_timer(int * timer,int sockfd) {

}

// 处理客户端的数据    
bool PIGG_WebServer::dealclientdata(){

}

// 处理信号
bool PIGG_WebServer::dealwithsignal(bool &timeout,bool &stop_server) {

}

// 处理读取
void PIGG_WebServer::dealwithread(int sockfd) {
    
}

// 处理写
void PIGG_WebServer::dealwithwrite(int sockfd) {
    // reactor
    // if (m_actor_model){
    //     if (timer){
    //         adjust_timer(timer);
    //     }
    // }
}
