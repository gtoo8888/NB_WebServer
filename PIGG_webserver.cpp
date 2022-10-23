#include "PIGG_webserver.h"
#include <ctime>
#include <fstream>

#define _DEBUG


const int MAX_FD = 65536;      //最大文件描述符

inline bool exist_file(const char* name){
    printf("%s\n",name);
    struct stat buffer;
    if(stat(name,&buffer) < 0){
        // LOG_WARN("%s","PIGG_WebServer::PIGG_WebServer():html file not exist");   // 日志还没有起来
        printf("%s","\n!!!!!!PIGG_WebServer::PIGG_WebServer():html file not exist!!!!!\n\n");
        return false;
    }else{
        printf("%s","html file is exist\n");
        return true;
    }
}

// 这是在构造PIGG_WebServer的时候检测的，这个时候日志还没有起来
PIGG_WebServer::PIGG_WebServer(){
    // 预先为每个可能的客户连接分配一个http_conn对象
    PIGG_http_users = new PIGG_http_conn[MAX_FD];
    PIGG_users_timer = new PIGG_client_data[MAX_FD];    // 需要分配一下空间，不然后面直接报错
}

// 析构的时候需要delete释放
PIGG_WebServer::~PIGG_WebServer(){
    close(PIGG_epollfd);
    close(PIGG_listenfd);
    close(PIGG_pipefd[1]);
    close(PIGG_pipefd[0]);
    // delete[] PIGG_http_users;
    // delete[] PIGG_users_timer;
    // delete PIGG_pool;
}
void PIGG_WebServer::init(PIGG_Config& temp_config){
    // 基础配置
    PIGG_port = temp_config.port;
    PIGG_user = temp_config.user;
    PIGG_password = temp_config.passwd;
    PIGG_databasename = temp_config.databasename;
    PIGG_actor_model = temp_config.actor_model;

    // 日志相关
    PIGG_close_log = temp_config.close_log; // 是否关闭日志,false才是开启日志
    PIGG_log_queue = temp_config.log_queue; // 开启日志队列
    PIGG_log_record_max = temp_config.log_record_max;         // 日志文件记录的最大条数 800000
    PIGG_block_queue_max_len = temp_config.block_queue_max_len;    // 最大阻塞队列长度 2000
    PIGG_block_queue_max_wait = temp_config.block_queue_max_wait;  // 最多有多少个消息可以在队列中等待 800

    // 线程池+mysql连接池
    PIGG_thread_num = temp_config.thread_num;
    PIGG_sql_num = temp_config.sql_num;


    //root文件夹路径
    char server_path[200];
    getcwd(server_path, 200);   // 检测当前可执行文件位置
    // printf("%s\n",server_path);
    const char* root_path = temp_config.html_root.c_str(); // 只需要将string首地址传过去就可以了 
    // exist_file(root_path);
    PIGG_root_path = (char *)malloc(strlen(server_path) + strlen(root_path) + 1);
    strcpy(PIGG_root_path, server_path);
    strcat(PIGG_root_path, root_path);  // fix:覆盖了
    #ifdef _DEBUG  // 为了vscode调试使用
        PIGG_root_path = "/data_hdd/PIGG_WebServer/html_root";  
        printf("_DEBUG\n");
    #endif
    exist_file(PIGG_root_path);
}

// 只有使用./build/PIGG_webserve才能正确创建
void PIGG_WebServer::log_write(){
    if(PIGG_log_queue == true){   // 是否写日志，开启就是写
        // 缓冲区长度2000, 一个日志文件记录的最大条数800000, 最大阻塞队列长度2000，最多有800个消息可以在队列中等待
        bool flag = PIGG_log::get_instance()->init("./ServerLog",PIGG_close_log, 
        PIGG_block_queue_max_wait, PIGG_log_record_max, PIGG_block_queue_max_len);
        if(flag == false){
            printf("PIGG_log::get_instance()->init error\n");
        }
    }else{
        // 关闭了阻塞队列
        bool flag = PIGG_log::get_instance()->init("./ServerLog",PIGG_close_log, 
        PIGG_block_queue_max_wait, PIGG_log_record_max, 0);
        // bool flag = PIGG_log::get_instance()->init("./PIGG_output/ServerLog",PIGG_close_log, 2000, 800000, 0);
        if(flag == false){
            printf("PIGG_log::get_instance()->init error\n");
        }
    }
}

void PIGG_WebServer::sql_pool(){
    //初始化数据库连接池
    PIGG_connPool = PIGG_connection_pool::GetInstance();
    PIGG_connPool->init("localhost",PIGG_user,PIGG_password,PIGG_databasename,3306,PIGG_sql_num,PIGG_close_log);

    //初始化数据库读取表
    PIGG_http_users->init_mysql_result(PIGG_connPool);


    MYSQL* mysql_test;
    PIGG_connection_RALL mysql_conn(&mysql_test,PIGG_connPool); // 只有使用PIGG_connPool才能取出来
    // 因为这是一个单例

    if(mysql_query(mysql_test,"select * from user")){
        // LOG_ERROR("SELECT error:%s\n",mysql_error(mysql));  
    }
}

void PIGG_WebServer::thread_pool(){
    PIGG_pool = new PIGG_threadpool<PIGG_http_conn>(PIGG_actor_model,PIGG_connPool,PIGG_thread_num);
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

    alarm(TIMESLOT);    //alarm定时触发SIGALRM信号

    PIGG_Utils::PIGG_pipfd = PIGG_pipefd;
    PIGG_Utils::PIGG_epollfd = PIGG_epollfd;
}

// 主程序循环，不断接收请求
void PIGG_WebServer::event_loop(){
    bool timeout = false;
    bool stop_server = false;

    while(!stop_server){
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
            }else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){//处理异常事件
                //服务器端关闭连接，移除对应的定时器
                PIGG_util_timer *timer = PIGG_users_timer[sockfd].PIGG_timer;
                deal_timer(timer,sockfd);
            }else if ((events[i].events & EPOLLIN) && ( sockfd == PIGG_pipefd[0])) {//处理定时器信号
                //接收到SIGALRM信号，timeout设置为True
                bool flag = deal_with_signal(timeout, stop_server);   // 处理信号
                if(flag == false){
                    LOG_ERROR("%s", "dealclientdata failure");
                }
            }else if (events[i].events & EPOLLIN) {     // 处理客户连接上接收到的数据
                deal_with_read(sockfd);
            }else if (events[i].events & EPOLLOUT) {    // 处理写数据
                deal_with_write(sockfd);
            }
        }
        //处理定时器为非必须事件，收到信号并不是立马处理
        //完成读写事件后，再进行处理
        if(timeout) {
            PIGG_webserver_utils.timer_handler();
            LOG_INFO("%s", "time tick");
            timeout = false;
        }
    }
}

// 判断时间
void PIGG_WebServer::adjust_timer(PIGG_util_timer* timer) {
    time_t cur = time(NULL);
    timer->expire = cur + 3 * TIMESLOT;
    PIGG_webserver_utils.PIGG_timer_lst.adjust_timer(timer);

    LOG_INFO("%s","adjust timer once");
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
        while(1){ // 需要循环接收数据
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
    //创建定时器临时变量，将该连接对应的定时器取出来
    PIGG_util_timer *timer = PIGG_users_timer[sockfd].PIGG_timer;

    MYSQL* mysql_test;
    PIGG_connection_RALL mysql_conn(&mysql_test,PIGG_connPool); // 只有使用PIGG_connPool才能取出来
    // 因为这是一个单例
    if(mysql_query(mysql_test,"select * from user")){
        // LOG_ERROR("SELECT error:%s\n",mysql_error(mysql));  
    }

    // reactor
    if(PIGG_actor_model == 1){  // 运行模式
        if(timer){
            adjust_timer(timer);
        }
        //若监测到读事件，将该事件放入请求队列
        PIGG_pool->append(PIGG_http_users + sockfd,0);
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
        if(PIGG_http_users[sockfd].read_once()){//读入对应缓冲区
            LOG_INFO("deal with the client(%s)",inet_ntoa(PIGG_http_users[sockfd].get_address()->sin_addr))

            // PIGG_http_users[sockfd].process();  // 这里面进行处理
            PIGG_pool->append_p(PIGG_http_users + sockfd);//若监测到读事件，将该事件放入请求队列
            //若有数据传输，则将定时器往后延迟3个单位
            //对其在链表上的位置进行调整
            if(timer){
                adjust_timer(timer);
            }
        }else{            //服务器端关闭连接，移除对应的定时器
            deal_timer(timer,sockfd);
        }
    }
}

// 处理写
void PIGG_WebServer::deal_with_write(int sockfd) {
    PIGG_util_timer *timer = PIGG_users_timer[sockfd].PIGG_timer;
    // reactor
    if (PIGG_actor_model){
        if (timer){
            adjust_timer(timer);
        }
        PIGG_pool->append(PIGG_http_users + sockfd,1);
        while(true){
            if(PIGG_http_users[sockfd].improv == 1){
                if(PIGG_http_users[sockfd].timer_flag == 1){
                    deal_timer(timer,sockfd);
                    PIGG_http_users[sockfd].timer_flag = 0;
                }
                PIGG_http_users[sockfd].improv = 0;
                break;
            }
        }
    }else {// proactor
        if(PIGG_http_users[sockfd].write()){
            LOG_INFO("send data to the client(%s)",inet_ntoa(PIGG_http_users[sockfd].get_address()->sin_addr))
            if(timer){
                adjust_timer(timer);
            }
        }else{
            deal_timer(timer,sockfd);
        }
    }
}

void PIGG_WebServer::PIGG_timer(int connfd,struct sockaddr_in clinet_address){
    PIGG_http_users[connfd].init(connfd,clinet_address,PIGG_root_path,PIGG_conn_trig_mode,PIGG_close_log,PIGG_user,PIGG_password,PIGG_databasename);
    
    //初始化client_data数据
    //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
    //初始化该连接对应的连接资源
    PIGG_users_timer[connfd].sockfd = connfd;
    PIGG_users_timer[connfd].address = clinet_address;
    

    PIGG_util_timer *timer = new PIGG_util_timer;    //创建定时器临时变量
    timer->user_data = &PIGG_users_timer[connfd];    //设置定时器对应的连接资源
    timer->PIGG_cb_func = PIGG_cb_func;    //设置回调函数

    time_t cur = time(NULL);
    timer->expire = cur + 3 * TIMESLOT;//设置绝对超时时间
    PIGG_users_timer[connfd].PIGG_timer = timer;//创建该连接对应的定时器，初始化为前述临时变量
    PIGG_webserver_utils.PIGG_timer_lst.add_timer(timer);//将该定时器添加到链表中
}
