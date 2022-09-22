#include "PIGG_webserver.h"
#include <ctime>

const int MAX_FD = 65536;      //最大文件描述符

PIGG_WebServer::PIGG_WebServer(){
    //http_conn类对象
    PIGG_http_users = new PIGG_http_conn[MAX_FD];

    //root文件夹路径
    char server_path[200];
    getcwd(server_path, 200);
    char root_path[6] = "/root";
    PIGG_root_path = (char *)malloc(strlen(server_path) + strlen(root_path) + 1);
    strcpy(PIGG_root_path, server_path);
    strcpy(PIGG_root_path, root_path);

    // PIGG_users_timer = new client_data[MAX_FD];
}

// 析构的时候需要delete释放
PIGG_WebServer::~PIGG_WebServer(){
    close(PIGG_epollfd);
    close(PIGG_listenfd);
    close(PIGG_pipefd[1]);
    close(PIGG_pipefd[0]);
    delete[] PIGG_http_users;
    // delete[] PIGG_users_timer;
    // delete[] PIGG_pool;
}

void PIGG_WebServer::init(int port, std::string user, std::string passWord, std::string databaseName,
                            bool close_log,bool log_queue){
    // 基础配置
    PIGG_port = port;
    PIGG_user = user;
    PIGG_password = passWord;
    PIGG_databasename = databaseName;

    // 日志相关
    PIGG_close_log = close_log; // 是否关闭日志,false才是开启日志
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
void PIGG_WebServer::sql_pool(){

}

void PIGG_WebServer::thread_pool(){

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
    // PF_INET IPV4协议
    // SOCK_STREAM TCP通讯
    PIGG_listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(PIGG_listenfd >= 0); // 如果小于0就报错

    // 是否开启优雅关闭连接，默认不开启
    if(PIGG_opt_LINGER == 0){
        struct linger tmp = {0, 1}; // l_onoff为0,l_linger被忽略，默认关闭，这个配置不起作用
        // 获取或者设置与某个套接字关联的选项
        setsockopt(PIGG_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
        // SOL_SOCKET 通用套接字选项
        // SO_LINGER 延迟关闭连接
    }else if(PIGG_opt_LINGER == 1){
        struct linger tmp = {1, 1}; // l_onoff为1,l_linger为超时时间，为1S，超时之前会发送所有数据
        setsockopt(PIGG_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PIGG_port);    //将一个无符号短整型数值转换为网络字节序，即大端模式(big-endian)　
    address.sin_addr.s_addr = htonl(INADDR_ANY);   // 将主机的无符号长整形数转换成网络字节顺序

    int flag = 1;
    // SO_REUSEADDR 重复使用本地端口和地址
    setsockopt(PIGG_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(PIGG_listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(PIGG_listenfd, 5);
    assert(ret >= 0);

    //epoll创建内核事件表
    epoll_event events[MAX_EVENT_NUMBER];
    PIGG_epollfd = epoll_create(5);
    assert(PIGG_epollfd != -1);

    PIGG_webserver_utils.addfd(PIGG_epollfd, PIGG_listenfd, false,PIGG_listen_trig_mode);
    PIGG_http_conn::PIGG_epollfd = PIGG_epollfd;

    // PF_UNIX 本地通讯
    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, PIGG_pipefd); // 建立一对匿名的已经连接的套接字
    assert(ret != -1);
    // 工具类,信号和描述符基础操作
    PIGG_webserver_utils.set_non_blocking(PIGG_pipefd[1]);
    PIGG_webserver_utils.addfd(PIGG_epollfd,PIGG_pipefd[0],false,0);

    PIGG_webserver_utils.addsig(SIGPIPE, SIG_IGN);
    PIGG_webserver_utils.addsig(SIGALRM, PIGG_webserver_utils.sig_handle,false);
    PIGG_webserver_utils.addsig(SIGALRM, PIGG_webserver_utils.sig_handle,false);

    alarm(TIMESLOT);

    PIGG_Utils::PIGG_pipfd = PIGG_pipefd;
    PIGG_Utils::PIGG_epollfd = PIGG_epollfd;
}

// 主程序循环，不断接收请求
void PIGG_WebServer::event_loop(){
    bool timeout = false;
    bool stop_server = false;

    while(!stop_server){
        LOG_INFO("%s", "epoll_wait");
        int number = epoll_wait(PIGG_epollfd, events, MAX_EVENT_NUMBER, -1);    // 如果没有信号进来，就在这里等着
        LOG_INFO("number:%d", number);
        if(number < 0 && errno != EINTR){
            LOG_ERROR("%s", "epoll failure");
            break;
        }

        for(int i = 0;i < number; i++){
            int sockfd = events[i].data.fd;
            // 要处理新到的客户端请求
            if (sockfd == PIGG_listenfd){   //如果是老的连接，就跳过了
                bool flag = deal_client_data(); // 处理客户端的数据
                if (flag == false)
                    continue;
            }else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                //服务器端关闭连接，移除对应的定时器
                PIGG_util_timer *timer = PIGG_users_timer[sockfd].PIGG_timer;
                deal_timer(timer,sockfd);
            }else if ((events[i].events & EPOLLIN) && ( sockfd == PIGG_pipefd[0])) {
                bool flag = deal_with_signal(timeout, stop_server);   // 处理信号
                if(flag == false){
                    LOG_ERROR("%s", "dealclientdata failure");
                }
            }else if (events[i].events & EPOLLIN) {     // 处理写数据
                deal_with_read(sockfd);
            }else if (events[i].events & EPOLLOUT) {    // 处理读数据
                deal_with_write(sockfd);
            }
        }
        if(timeout) {
            PIGG_webserver_utils.timer_handler();
            LOG_INFO("%s", "time tick");
            timeout = false;
        }
    }
}

// 判断时间
void PIGG_WebServer::adjust_timer(PIGG_util_timer* timer) {

}

// 处理时间
void PIGG_WebServer::deal_timer(PIGG_util_timer* timer,int sockfd) {
    timer->PIGG_cb_func(&PIGG_users_timer[sockfd]);
    if(timer){
        PIGG_webserver_utils.PIGG_timer_lst.del_timer(timer);
    }
    LOG_INFO("close fd %d",PIGG_users_timer[sockfd].sockfd);
}

// 处理客户端的数据    
bool PIGG_WebServer::deal_client_data(){
    struct sockaddr_in PIGG_client_addr;
    socklen_t PIGG_client_addr_len = sizeof(PIGG_client_addr);
    if(PIGG_listen_trig_mode == 0){
        int connfd = accept(PIGG_listenfd, (struct sockaddr *)&PIGG_client_addr,&PIGG_client_addr_len);
        if(connfd < 0){
            LOG_ERROR("%s:error is:%d","accept error",errno);
            return false;
        }
        if(PIGG_http_conn::PIGG_user_count >= MAX_FD){
            PIGG_webserver_utils.show_error(connfd, "Internal server busy");
            LOG_ERROR("%s","Internal server busy");
            return false;
        }
        PIGG_timer(connfd, PIGG_client_addr);
    }else{
        while(1){
            int connfd = accept(PIGG_listenfd, (struct sockaddr *)&PIGG_client_addr,&PIGG_client_addr_len);
            if(connfd < 0){
                LOG_ERROR("%s:error is:%d","accept error",errno);
                break;
            }
            if(PIGG_http_conn::PIGG_user_count >= MAX_FD){
                PIGG_webserver_utils.show_error(connfd, "Internal server busy");
                LOG_ERROR("%s","Internal server busy");
                return false;
            }
            PIGG_timer(connfd, PIGG_client_addr);
        }
        return false;
    }
    return true;
}

// 处理信号
bool PIGG_WebServer::deal_with_signal(bool &timeout,bool &stop_server) {
    int ret = 0;
    int sig;
    char signals[1024];
    ret = recv(PIGG_pipefd[0],signals,sizeof(signals),0);
    if(ret == -1){
        return false;
    }else if(ret == 0){
        return false;
    }else{
        for(int i = 0;i < ret;i++){
            switch (signals[i]){
                case SIGALRM:{  // SIGALRM是在定时器终止时发送给进程的信号。
                    timeout = true;
                    break;
                }
                case SIGTERM:{  // SIGTERM信号是用于终止程序的通用信号
                    stop_server = true;
                    break;
                }
            }
        }
    }
    return true;
}

// 处理读取
void PIGG_WebServer::deal_with_read(int sockfd) {
    PIGG_util_timer *timer = PIGG_users_timer[sockfd].PIGG_timer;

    // reactor
    if(PIGG_actor_model == 1){  // 运行模式
        if(timer){
            adjust_timer(timer);
        }
        // PIGG_pool->
        while(true){
            if(PIGG_http_users[sockfd].improv){
                if(PIGG_http_users[sockfd].timer_flag){
                    deal_timer(timer,sockfd);
                    PIGG_http_users[sockfd].timer_flag = 0;
                }
                PIGG_http_users[sockfd].improv = 0;
                break;
            }
        }
    }else{// proactor
        if(PIGG_http_users[sockfd].read_once()){
            LOG_INFO("deal with the client(%s)",inet_ntoa(PIGG_http_users[sockfd].get_address()->sin_addr))
            // PIGG_pool->
            if(timer){
                adjust_timer(timer);
            }
        }else{
            deal_timer(timer,sockfd);
        }
    }
}

// 处理写
void PIGG_WebServer::deal_with_write(int sockfd) {
    // reactor
    // if (m_actor_model){
    //     if (timer){
    //         adjust_timer(timer);
    //     }
    // }
}

void PIGG_WebServer::PIGG_timer(int connfd,struct sockaddr_in clinet_address){
    PIGG_http_users[connfd].init(connfd,clinet_address,PIGG_root_path,PIGG_conn_trig_mode,PIGG_close_log,PIGG_user,PIGG_password,PIGG_databasename);
    
    //初始化client_data数据
    //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
    PIGG_users_timer[connfd].address = clinet_address;
    PIGG_users_timer[connfd].sockfd = connfd;

    PIGG_util_timer *timer = new PIGG_util_timer;
    timer->user_data = &PIGG_users_timer[connfd];
    timer->PIGG_cb_func = PIGG_cb_func;
    time_t cur = time(NULL);
    timer->expire = cur + 3 * TIMESLOT;
    PIGG_users_timer[connfd].PIGG_timer = timer;
    PIGG_webserver_utils.PIGG_timer_lst.add_timer(timer);
}
