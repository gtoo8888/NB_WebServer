#ifndef _PIGG_HTTP_
#define _PIGG_HTPP_


#include <iostream>
#include <sys/epoll.h>  // EPOLLIN
#include <arpa/inet.h>  // sockaddr_in
#include <cstring>

#include "../CGImysql/sql_connection_pool.h"
#include "../PIGG_log/PIGG_log.h"


class PIGG_http_conn{
public:
    static const int FILE_NAME_LEN = 200; // 文件名长度
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;

    enum PIGG_METHOD{
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };

    enum PIGG_CHECK_STATE{

    };

    enum PIGG_HTTP_CODE{
        NO_REQUEST,              
        GET_REQUEST,
        BAD_REQUEST,            // 坏的请求
        FORBIDDEN_REQUEST,      // 禁止请求 forbidden 
        FILE_REQUEST,          
        INTERNAL_ERROR,         // 内部错误 internal
        CLOSED_CONNECTION,      // 关闭连接
        NO_SOURCE               // 没有请求的资源
    };

    enum PIGG_LINE_STATUS{

    };
public:
    PIGG_http_conn() {}
    ~PIGG_http_conn() {}

public:
    void init(int sockfd,const sockaddr_in &addr, char *root, int TRIGMode,int close_log, std::string user,std::string passwd,std::string sqlname);
    void close_conn(bool real_close);
    void process();
    bool read_once();
    bool write();
    void init_mysql_result(PIGG_connection_pool *connPool);
    sockaddr_in *get_address(){
        return &PIGG_address;
    }
    int timer_flag;
    int improv;

private:
    void init();
    PIGG_LINE_STATUS parse_line();
    PIGG_HTTP_CODE parse_request_line(char *text);
    PIGG_HTTP_CODE parse_headers(char *text);
    PIGG_HTTP_CODE parse_content(char *text);
    PIGG_HTTP_CODE process_read();
    PIGG_HTTP_CODE do_request();


    void unmap();
    bool add_response(const char * format, ...);
    bool add_status_line(int status,const char *title);
    bool add_headers(int content_len);
    bool add_content_length(int content_len);
    bool add_content_type();
    bool add_linger();
    bool add_blank_line();
    bool add_content(const char *content);
    bool process_write(PIGG_HTTP_CODE read_ret);


private:
    int PIGG_sockfd;    // 文件描述符
    int PIGG_TRIGMode;  //触发模式
    int PIGG_epollfd;
    sockaddr_in PIGG_address;

    char PIGG_write_buf[WRITE_BUFFER_SIZE]; // buffer中的内容
    int PIGG_write_idx; // 写的内容的下标，查看是否超过了BUFFER的最大值 
    int PIGG_linger;        // 是否使用keep-alive

    
};

#endif
