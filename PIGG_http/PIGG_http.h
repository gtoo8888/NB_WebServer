#ifndef _PIGG_HTTP_
#define _PIGG_HTPP_


#include <iostream>
#include <sys/epoll.h>  // EPOLLIN
#include <arpa/inet.h>  // sockaddr_in
#include <sys/stat.h>
#include <cstring>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/unistd.h> // close()


#include "../CGImysql/sql_connection_pool.h"
#include "../PIGG_log/PIGG_log.h"
#include "../PIGG_lock/PIGG_lock.h"


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

    enum PIGG_CHECK_STATE{          // 核对状态
        CHECK_STATE_REQUEST_LINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };

    enum PIGG_HTTP_CODE{
        NO_REQUEST,             // 响应没有问题   
        GET_REQUEST,
        BAD_REQUEST,            // 坏的请求
        FORBIDDEN_REQUEST,      // 禁止请求 forbidden 
        FILE_REQUEST,          
        INTERNAL_ERROR,         // 内部错误 internal
        CLOSED_CONNECTION,      // 关闭连接
        NO_SOURCE               // 没有请求的资源
    };

    enum PIGG_LINE_STATUS{
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };
public:
    PIGG_http_conn() {}
    ~PIGG_http_conn() {}

public:
    void init(int sockfd,const sockaddr_in &addr, char *root, int trig_mode,int close_log, std::string user,std::string passwd,std::string sqlname);
    void close_conn(bool real_close);
    void process();     //读取浏览器端发来的全部数据
    bool read_once();   //响应报文写入函数
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

    char *get_line() {return PIGG_read_buf + PIGG_start_line;};

public:
    static int PIGG_user_count;  // 在定时器中要用
    static int PIGG_epollfd;    // 在主webserver中使用
    MYSQL* mysql;
    int PIGG_state;     //读为0, 写为1

private:
    int PIGG_sockfd;    // 文件描述符
    int PIGG_trig_mode;  //触发模式

    char* PIGG_url;
    char* PIGG_version;
    PIGG_CHECK_STATE PIGG_check_status;
    int PIGG_content_length;
    char* PIGG_host;
    int PIGG_cgi; // 是否启用POST
    int PIGG_read_idx;
    int PIGG_checked_idex;
    char* PIGG_string; // 储存请求头数据
    sockaddr_in PIGG_address;
    PIGG_METHOD PIGG_method;
    int PIGG_start_line;
    char* doc_root; // 存放html的文件夹
    struct stat PIGG_file_stat;
    struct iovec PIGG_iv[2];
    int PIGG_iv_count;
    char* PIGG_file_address;

    char PIGG_real_file[FILE_NAME_LEN];
    char PIGG_read_buf[READ_BUFFER_SIZE];
    char PIGG_write_buf[WRITE_BUFFER_SIZE]; // buffer中的内容
    int PIGG_write_idx; // 写的内容的下标，查看是否超过了BUFFER的最大值 
    int PIGG_linger;        // 是否使用keep-alive

    int PIGG_close_log;
    char* PIGG_doc_root;


    int bytes_to_send;
    int bytes_have_send;
    

    // sql相关
    char sql_user[100];
    char sql_passwd[100];
    char sql_name[100];
};

#endif
