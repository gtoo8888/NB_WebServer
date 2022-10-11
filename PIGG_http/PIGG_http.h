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
    static const int FILE_NAME_LEN = 200;  //设置读取文件的名称m_real_file大小
    static const int READ_BUFFER_SIZE = 2048; //设置读缓冲区m_read_buf大小
    static const int WRITE_BUFFER_SIZE = 1024;  //设置写缓冲区m_write_buf大小

    enum PIGG_METHOD{ //报文的请求方法，本项目只用到GET和POST
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

    enum PIGG_CHECK_STATE{          //主状态机的状态
        CHECK_STATE_REQUEST_LINE = 0,   // 解析请求行
        CHECK_STATE_HEADER,             // 解析请求头
        CHECK_STATE_CONTENT             // 解析消息体，仅用于解析POST请求
    };

    enum PIGG_HTTP_CODE{         //报文解析的结果
        NO_REQUEST,             // 请求不完整，需要继续读取请求报文数据
        GET_REQUEST,            // 获得了完整的HTTP请求
        BAD_REQUEST,            // HTTP请求报文有语法错误
        FORBIDDEN_REQUEST,      // 禁止请求 forbidden 
        FILE_REQUEST,          
        INTERNAL_ERROR,         // 服务器内部错误 internal 该结果在主状态机逻辑switch的default下，一般不会触发
        CLOSED_CONNECTION,      // 关闭连接
        NO_SOURCE               // 没有请求的资源
    };

    enum PIGG_LINE_STATUS{  // 从状态机的状态
        LINE_OK = 0,        // 完整读取一行
        LINE_BAD,           // 报文语法有误
        LINE_OPEN           // 读取的行不完整
    };
public:
    PIGG_http_conn() {}
    ~PIGG_http_conn() {}

public:
    //初始化套接字地址，函数内部会调用私有方法init
    void init(int sockfd,const sockaddr_in &addr, char *root, int trig_mode,int close_log, std::string user,std::string passwd,std::string sqlname);
    void close_conn(bool real_close);//关闭http连接
    void process();     //读取浏览器端发来的全部数据
    bool read_once();   //响应报文写入函数
    bool write();
    void init_mysql_result(PIGG_connection_pool *connPool); //同步线程初始化数据库读取表
    sockaddr_in *get_address(){
        return &PIGG_address;
    }
    int timer_flag;
    int improv;

private:
    void init();
    PIGG_HTTP_CODE parse_request_line(char *text);  //主状态机解析报文中的请求行数据
    PIGG_HTTP_CODE parse_headers(char *text);   //主状态机解析报文中的请求头数据
    PIGG_HTTP_CODE parse_content(char *text);   //主状态机解析报文中的请求内容
    PIGG_HTTP_CODE process_read();   //从m_read_buf读取，并处理请求报文
    PIGG_HTTP_CODE do_request();    //生成响应报文
    PIGG_LINE_STATUS parse_line();   //从状态机读取一行，分析是请求报文的哪一部分


    void unmap();
    bool process_write(PIGG_HTTP_CODE read_ret);    //向m_write_buf写入响应报文数据
     //根据响应报文格式，生成对应8个部分，以下函数均由do_request调用
    bool add_response(const char * format, ...);
    bool add_status_line(int status,const char *title);
    bool add_headers(int content_len);
    bool add_content_length(int content_len);
    bool add_content_type();
    bool add_linger();
    bool add_blank_line();
    bool add_content(const char *content);

    //m_start_line是已经解析的字符
    //get_line用于将指针向后偏移，指向未处理的字符
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
    PIGG_CHECK_STATE PIGG_check_status; //主状态机的状态
    PIGG_METHOD PIGG_method;     //请求方法
    int PIGG_content_length;
    char* PIGG_host;
    int PIGG_cgi; // 是否启用POST
    char* PIGG_string; // 储存请求头数据
    sockaddr_in PIGG_address;
    int PIGG_start_line;    //m_read_buf中已经解析的字符个数
    char* PIGG_doc_root; // 存放html的文件夹
    int PIGG_iv_count;

    char PIGG_real_file[FILE_NAME_LEN];
    char PIGG_read_buf[READ_BUFFER_SIZE];   //存储读取的请求报文数据
    int PIGG_read_idx;  //缓冲区中m_read_buf中数据的最后一个字节的下一个位置
    int PIGG_checked_idx;  //m_read_buf读取的位置m_checked_idx
    char PIGG_write_buf[WRITE_BUFFER_SIZE]; //存储发出的响应报文数据
    int PIGG_write_idx; // 写的内容的下标，查看是否超过了BUFFER的最大值 
    int PIGG_linger;        // 是否使用keep-alive

    int PIGG_close_log;

    int bytes_to_send;  //剩余发送字节数
    int bytes_have_send;    //已发送字节数
    
    char* PIGG_file_address;    //读取服务器上的文件地址
    struct stat PIGG_file_stat;
    struct iovec PIGG_iv[2];     //io向量机制iovec
    // sql相关
    char sql_user[100];
    char sql_passwd[100];
    char sql_name[100];
};

#endif
