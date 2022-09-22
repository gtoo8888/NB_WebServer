#ifndef _PIGG_LST_TIMER_
#define _PIGG_LST_TIMER_


// 定时器处理非活动连接
// ===============
// 由于非活跃连接占用了连接资源，严重影响服务器的性能，通过实现一个服务器定时器，处理这种非活跃连接，释放连接资源。利用alarm函数周期性地触发SIGALRM信号,该信号的信号处理函数利用管道通知主循环执行定时器链表上的定时任务.
// > * 统一事件源
// > * 基于升序链表的定时器
// > * 处理非活动连接

#include <arpa/inet.h>  
#include <cstdio>

class PIGG_util_timer;

struct PIGG_client_data{     // 定时器中间存的数据
    sockaddr_in address;
    int sockfd;
    PIGG_util_timer *PIGG_timer;
};

class PIGG_util_timer{      // 微服务时间？
public:
    PIGG_util_timer() : prev(NULL), next(NULL){}
public:
    time_t expire;      // 到期时间

    void (* PIGG_cb_func)(PIGG_client_data *);
    PIGG_client_data *user_data;
    PIGG_util_timer* prev;
    PIGG_util_timer* next;
};

class PIGG_sort_timer_lst{      // 对时间进行处理,做加减
public:
    PIGG_sort_timer_lst();
    ~PIGG_sort_timer_lst();

    void add_timer(PIGG_util_timer *timer);
    void adjust_timer(PIGG_util_timer *timer);
    void del_timer(PIGG_util_timer *timer);
    void tick();
private:
    void add_timer(PIGG_util_timer *timer, PIGG_util_timer* lst_head);      // 增加时间？

    PIGG_util_timer *head;  
    PIGG_util_timer *tail;  // 结尾
};

class PIGG_Utils{       // 最大的微服务？
public:
    PIGG_Utils(){}
    ~PIGG_Utils(){}

    void init(int timeslot);

    //对文件描述符设置非阻塞
    int set_non_blocking(int fd);

    //将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
    void addfd(int epollfd, int fd, bool one_shot, int TrigMode);
    //信号处理函数
    static void sig_handle(int sig);
    //设置信号函数
    void addsig(int sig, void(handler)(int), bool restart = true);
    //定时处理任务，重新定时以不断触发SIGALRM信号
    void timer_handler();

    void show_error(int connfd, const char* info);

public:
    static int *PIGG_pipfd;
    PIGG_sort_timer_lst PIGG_timer_lst;
    static int PIGG_epollfd;
    int PIGG_TimeSlot;
};

void PIGG_cb_func(PIGG_client_data *user_data);


#endif /* _PIGG_LST_TIMER_*/ 