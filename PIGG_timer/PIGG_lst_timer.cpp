#include "PIGG_lst_timer.h"
#include <ctime>
#include <sys/epoll.h>
#include <sys/unistd.h>
#include <cassert>
#include <signal.h>
#include "../PIGG_http/PIGG_http.h"


/*************************************************PIGG_sort_timer_lst*********************************************/
PIGG_sort_timer_lst::PIGG_sort_timer_lst(){
    head = NULL;
    tail = NULL;
}

// 链表被销毁时，删除所有定时器
PIGG_sort_timer_lst::~PIGG_sort_timer_lst(){
    PIGG_util_timer *tmp = head;
    while(tmp){
        head = tmp->next;
        delete tmp;
        tmp = head;
    }
}

void PIGG_sort_timer_lst::add_timer(PIGG_util_timer *timer){
    if(!timer)
        return;
    if(!head){
        head = tail = timer;
        return;
    }
    //如果新的定时器超时时间小于当前头部结点
    //直接将当前定时器结点作为头部结点
    if(timer->expire < head->expire){
        timer->next = head;
        head->prev = timer;
        head = timer;
        return;
    }
    //否则调用重载函数，调整内部结点
    // 把定时器插入链表中合适的位置，保证链表的升序特性
    add_timer(timer,head);
}

//添加定时器，内部调用私有成员add_timer
void PIGG_sort_timer_lst::add_timer(PIGG_util_timer *timer, PIGG_util_timer* lst_head){
    PIGG_util_timer *prev = lst_head;
    PIGG_util_timer *tmp = prev->next;
    while(tmp){
        if(timer->expire < tmp->expire){
            prev->next = timer;
            timer->next = tmp;
            tmp->prev = timer;
            timer->prev = prev;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    if(!tmp){
        prev->next = timer;
        timer->prev = prev;
        timer->next = NULL;
        tail = timer;
    }
}

//调整定时器，任务发生变化时，调整定时器在链表中的位置
void PIGG_sort_timer_lst::adjust_timer(PIGG_util_timer *timer){
    if(!timer){
        return;
    }
    PIGG_util_timer *tmp = timer->next;
    
    //被调整的定时器在链表尾部,定时器超时值仍然小于下一个定时器超时值，不调整
    if(!tmp || (timer->expire < tmp->expire)){
        return;
    }
    //被调整定时器是链表头结点，将定时器取出，重新插入
    if(timer == head){
        head = head->next;
        head->prev = NULL;
        timer->next = NULL;
        add_timer(timer,head);
    }else{//被调整定时器不是链表头结点，将改定时器从链表中取出并重新插入原来所在位置之后的部分链表中
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        add_timer(timer,head);
    }
}

void PIGG_sort_timer_lst::del_timer(PIGG_util_timer *timer){
    if(!timer){
        return;
    }
    //链表中只有一个定时器，需要删除该定时器
    if((timer == head) && (timer == tail)){
        delete timer;
        head = NULL;
        tail = NULL;
        return;
    }
     //被删除的定时器为头结点
    if(timer == head){
        head = head->next;
        head->prev = NULL;
        delete timer;
        return;
    }
     //被删除的定时器为尾结点
    if(timer == tail){
        tail = tail->prev;
        tail->next = NULL;
        delete timer;
        return;
    }
    //被删除的定时器在链表内部，常规链表结点删除
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;
}

void PIGG_sort_timer_lst::tick(){
    if(!head){
        return;
    }

    //获取当前时间
    time_t cur = time(NULL);
    PIGG_util_timer* tmp = head;
    while(tmp){//遍历定时器链表
        //链表容器为升序排列
        //当前时间小于定时器的超时时间，后面的定时器也没有到期
        if(cur < tmp->expire){
            break;
        }
        //当前定时器到期，则调用回调函数，执行定时事件
        tmp->PIGG_cb_func(tmp->user_data);
        //将处理后的定时器从链表容器中删除，并重置头结点
        head = tmp->next;
        if(head){
            head->prev = NULL;
        }
        delete tmp;
        tmp = head;
    }

}




/*************************************************PIGG_Utils*********************************************/

void PIGG_Utils::init(int timeslot){

}

//对文件描述符设置非阻塞
int PIGG_Utils::set_non_blocking(int fd){
    int old_option = fcntl(fd,F_GETFL); // F_GETFL得到套接字属性
    int new_option = old_option | O_NONBLOCK ;  // O_NONBLOCK 设置套接字为非阻塞IO
    fcntl(fd, F_SETFL, new_option); // F_SETFL 用来设置属性
    return old_option;
}

//将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
void PIGG_Utils::addfd(int epollfd, int fd, bool one_shot, int TrigMode){
    epoll_event event;
    event.data.fd = fd;

    if(TrigMode == 1)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else
        event.events = EPOLLIN | EPOLLRDHUP;
    
    if(one_shot)
        event.events |= EPOLLONESHOT;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    set_non_blocking(fd);   //对文件描述符设置非阻塞
}

//信号处理函数
void PIGG_Utils::sig_handle(int sig){
    //为保证函数的可重入性，保留原来的errno
    //可重入性表示中断后再次进入该函数，环境变量与之前相同，不会丢失数据
    int save_errno = errno;
    int msg = sig;
    //将信号值从管道写端写入，传输字符类型，而非整型
    send(PIGG_pipfd[1],(char*)&msg,1,0);
    //将原来的errno赋值为当前的errno
    errno = save_errno;
}

// 设置信号函数
void PIGG_Utils::addsig(int sig, void(handler)(int), bool restart){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if(restart)
        sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

//定时处理任务，重新定时以不断触发SIGALRM信号
void PIGG_Utils::timer_handler(){
    PIGG_timer_lst.tick();
    alarm(PIGG_TimeSlot);
}

void PIGG_Utils::show_error(int connfd, const char* info){
    send(connfd, info, strlen(info),0);
    close(connfd);
}

int *PIGG_Utils::PIGG_pipfd = 0;
int PIGG_Utils::PIGG_epollfd = 0;

class PIGG_Utils;

/******************************************定时器回调函数**************************/
void PIGG_cb_func(PIGG_client_data *user_data){
    //删除非活动连接在socket上的注册事件
    epoll_ctl(PIGG_Utils::PIGG_epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);//关闭文件描述符
    PIGG_http_conn::PIGG_user_count--;//减少连接数
}