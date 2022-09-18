#include "PIGG_http.h"


/*********************************内部使用***********************************************/

//对文件描述符设置非阻塞
int setnonblocking(int fd){

}

// 将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
void addfd(int epollfd,int fd, bool one_shot, int TRIGMode){

}

// 从内核时间表删除描述符
void remoefd(int epollfd, int fd){

}

// 将事件重置为EPOLLONESHOT
// 同一SOCKET只能被一个线程处理，不会跨越多个线程
// ************重要函数，需要理解***************/
void modfd(int epollfd, int fd, int ev ,int TrigMode){
    epoll_event event;
    event.data.fd = fd;

    if(TrigMode == 1)
        event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    else   
        event.events = ev | EPOLLONESHOT | EPOLLRDHUP;

    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}


/********************************************************************************/


void PIGG_http_conn::init_mysql_result(PIGG_connection_pool *connPool){

}


//关闭连接，关闭一个连接，客户总量减一
void PIGG_http_conn::close_conn(bool real_close){

}

void PIGG_http_conn::init(int sockfd,const sockaddr_in &addr, char *root, int TRIGMode,
    int close_log, std::string user,std::string passwd,std::string sqlname){

}

void PIGG_http_conn::init(){

}

// 对每行进行分析
PIGG_http_conn::PIGG_LINE_STATUS PIGG_http_conn::parse_line(){

}


//循环读取客户数据，直到无数据可读或对方关闭连接
//非阻塞ET工作模式下，需要一次性将数据读完
bool PIGG_http_conn::read_once(){

}


void PIGG_http_conn::unmap(){

}
bool PIGG_http_conn::write(){

}


// 整个的处理过程
// EPOLLIN/EPOLLOUT需要再去理解
void PIGG_http_conn::process(){
    PIGG_HTTP_CODE read_ret = process_read();   // 先把请求读取进来
    if(read_ret == NO_REQUEST){
        modfd(PIGG_epollfd, PIGG_sockfd, EPOLLIN, PIGG_TRIGMode);
    }
    bool wriet_ret = process_write(read_ret);   // 再将要发送的东西写进去,根据得到的不同请求进行不同的发送
    if(!wriet_ret){
        // close_conn(true);
    }
    modfd(PIGG_epollfd, PIGG_sockfd, EPOLLOUT, PIGG_TRIGMode);
}



