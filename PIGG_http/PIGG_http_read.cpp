#include "PIGG_http.h"
#include <map>

PIGG_locker PIGG_http_lock;
std::map<std::string,std::string> users;


//从状态机，用于分析出一行内容
//返回值为行的读取状态，有LINE_OK,LINE_BAD,LINE_OPEN

//m_read_idx指向缓冲区m_read_buf的数据末尾的下一个字节
//m_checked_idx指向从状态机当前正在分析的字节
PIGG_http_conn::PIGG_LINE_STATUS PIGG_http_conn::parse_line(){
    char temp;
    for(; PIGG_checked_idx < PIGG_read_idx;++PIGG_checked_idx){
        temp = PIGG_read_buf[PIGG_checked_idx];//temp为将要分析的字节
        if(temp == '\r'){//如果当前是\r字符，则有可能会读取到完整行
            if((PIGG_checked_idx + 1) == PIGG_read_idx)//下一个字符达到了buffer结尾，则接收不完整，需要继续接收
                return LINE_OPEN;
            else if(PIGG_read_buf[PIGG_checked_idx + 1] == '\n'){//下一个字符是\n，将\r\n改为\0\0
                PIGG_read_buf[PIGG_checked_idx++] = '\0';
                PIGG_read_buf[PIGG_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD; //如果都不符合，则返回语法错误
        }else if(temp == '\n'){
        //如果当前字符是\n，也有可能读取到完整行
        //一般是上次读取到\r就到buffer末尾了，没有接收完整，再次接收时会出现这种情况
            if(PIGG_checked_idx > 1 && PIGG_read_buf[PIGG_read_idx - 1] == '\n'){//前一个字符是\r，则接收完整
                PIGG_read_buf[PIGG_checked_idx - 1] = '\0';
                PIGG_read_buf[PIGG_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    //并没有找到\r\n，需要继续接收
    return LINE_OPEN;
}

//解析http请求行，获得请求方法，目标url及http版本号
PIGG_http_conn::PIGG_HTTP_CODE PIGG_http_conn::parse_request_line(char *text){
    /*************************************处理url******************************/
    PIGG_url = strpbrk(text, " \t");    // str1 中第一个匹配字符串 str2 中字符的字符数
    if(!PIGG_url){
        return BAD_REQUEST;
    }
    *PIGG_url++ = '\0';
    char* method = text;
    if(strcasecmp(method, "GET") == 0)  // 判断字符串是否相等的函数
        PIGG_method = GET;
    else if(strcasecmp(method, "POST") == 0){
        PIGG_method = POST;
        PIGG_cgi = 1;
    }else
        return BAD_REQUEST;
    PIGG_url += strspn(PIGG_url, " \t");    //str1 中第一个不在字符串 str2 中出现的字符下标。

    /*************************************处理version******************************/
    PIGG_version = strpbrk(PIGG_url, " \t");
    if(!PIGG_version)
        return BAD_REQUEST;
    *PIGG_version++ = '\0';
    PIGG_version += strspn(PIGG_version, " \t");
    if(strcasecmp(PIGG_version, "HTTP/1.1") != 0)
        return BAD_REQUEST;
    if(strncasecmp(PIGG_version,"http://",7) == 0){     // 和上面的strcasecmp不一样
        PIGG_url += 7;
        PIGG_url = strchr(PIGG_url, '/');
    }
    if(strncasecmp(PIGG_url, "https://",8) == 0){
        PIGG_url += 8;
        PIGG_url = strchr(PIGG_url, '/');
    }

    if(!PIGG_url || PIGG_url[0] != '/')
        return BAD_REQUEST;
    if(strlen(PIGG_url) == 1)//当url为/时，显示判断界面
        strcat(PIGG_url, "judge.html");
    PIGG_check_status = CHECK_STATE_HEADER;     // 核对头部
    return NO_REQUEST;
}

//解析http请求的一个头部信息
PIGG_http_conn::PIGG_HTTP_CODE PIGG_http_conn::parse_headers(char *text){
    if(text[0] == '\0'){
        if(PIGG_content_length != 0){
            PIGG_check_status = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }else if(strncasecmp(text, "Connection:", 11) == 0){
        text += 11;
        text += strspn(text, " \t");
        if(strcasecmp(text, "keep-alive") == 0){
            PIGG_linger = true;
        }
    }else if(strncasecmp(text, "Content-length:",15) == 0){
        text += 15;
        text += strspn(text," \t");
        PIGG_content_length = atol(text);   // 把参数 str 所指向的字符串转换为一个长整数
    }else if(strncasecmp(text, "Host:", 5) == 0){
        text += 5;
        text += strspn(text, " \t");
        PIGG_host = text;
    }else{
        LOG_INFO("oop!unknow header: %s",text);
    }
    return NO_REQUEST;
}

//判断http请求是否被完整读入
PIGG_http_conn::PIGG_HTTP_CODE PIGG_http_conn::parse_content(char *text){
    if(PIGG_read_idx >= (PIGG_content_length + PIGG_checked_idx)){
        text[PIGG_content_length] = '\0';
        PIGG_string = text;
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

PIGG_http_conn::PIGG_HTTP_CODE PIGG_http_conn::do_request(){
    strcpy(PIGG_real_file, PIGG_doc_root);   // 把 doc_root 所指向的字符串复制到 PIGG_real_file
    int len = strlen(PIGG_doc_root);
    const char *p = strrchr(PIGG_url, '/');

    //处理cgi
    if(PIGG_cgi == 1 && (*(p+1) == '2' || *(p+1) == '3')){
        char flag = PIGG_url[1];
        char *PIGG_url_real = (char*)malloc(sizeof(char) * 200);
        strcpy(PIGG_url_real, "/");
        strcat(PIGG_url_real,PIGG_url + 2);
        strncpy(PIGG_real_file + len, PIGG_url_real, FILE_NAME_LEN - len - 1);
        free(PIGG_url_real);

        //将用户名和密码提取出来
        //user=123&passwd=123
        char name[100],passwd[100];
        int i = 0;
        for(i = 5;PIGG_string[i] != '&';++i)
            name[i - 5] = PIGG_string[i];
        name[i-5] = '\0';

        int j = 0;
        for(i = i + 10;PIGG_string[i] != '\0';j++)
            passwd[j] = PIGG_string[i];
        passwd[j] = '\0';

        if(*(p+1) == '3'){
            char *sql_insert = (char *)malloc(sizeof(char) * 200) ;
            strcpy(sql_insert, "insert into user(username,passwd) VALUES(");
            strcpy(sql_insert, "'");
            strcpy(sql_insert, name);
            strcpy(sql_insert, ",'");
            strcpy(sql_insert, passwd);
            strcpy(sql_insert, ")");

            if (users.find(name) == users.end()){   // 检查是否重名，如果不重名，插入用户名
                // PIGG_http_lock.PIGG_lock();
                int res = mysql_query(mysql, sql_insert);
                users.insert(std::pair<std::string,std::string>(name,passwd));
                PIGG_http_lock.PIGG_unlock();

                if(!res)
                    strcpy(PIGG_url, "/log.html");
                else   
                    strcpy(PIGG_url, "/registerError.html");
            }
            else    // 如果重名了，直接弹出错误页面
                strcpy(PIGG_url, "/registerError.html");
        }else if(*(p+1) == '2'){
            if(users.find(name) != users.end() && users[name] == passwd)
                strcpy(PIGG_url, "/Welcome.html");
            else
                strcpy(PIGG_url, "/logError.html");
        }
    }

    if(*(p + 1) == '0'){
        char* PIGG_url_real = (char*)malloc(sizeof(char) * 200);
        strcpy(PIGG_url_real, "/register.html");
        strncpy(PIGG_real_file + len, PIGG_url_real, strlen(PIGG_url_real));
        free(PIGG_url_real);
    }else if(*(p + 1) == '1'){
        char* PIGG_url_real = (char*)malloc(sizeof(char) * 200);
        strcpy(PIGG_url_real, "/log.html");
        strncpy(PIGG_real_file + len, PIGG_url_real, strlen(PIGG_url_real));
        free(PIGG_url_real);
    }else if(*(p + 1) == '5'){
        char* PIGG_url_real = (char*)malloc(sizeof(char) * 200);
        strcpy(PIGG_url_real, "/picture.html");
        strncpy(PIGG_real_file + len, PIGG_url_real, strlen(PIGG_url_real));
        free(PIGG_url_real);
    }else if(*(p + 1) == '6'){
        char* PIGG_url_real = (char*)malloc(sizeof(char) * 200);
        strcpy(PIGG_url_real, "/video.html");
        strncpy(PIGG_real_file + len, PIGG_url_real, strlen(PIGG_url_real));
        free(PIGG_url_real);
    }else if(*(p + 1) == '7'){
        char* PIGG_url_real = (char*)malloc(sizeof(char) * 200);
        strcpy(PIGG_url_real, "/fans.html");
        strncpy(PIGG_real_file + len, PIGG_url_real, strlen(PIGG_url_real));
        free(PIGG_url_real);
    }else   
        strncpy(PIGG_real_file + len, PIGG_url, FILE_NAME_LEN - len - 1);

    if(stat(PIGG_real_file, &PIGG_file_stat) < 0)       // 获取文件信息
        return NO_SOURCE;   // 没有资源

    if(!(PIGG_file_stat.st_mode & S_IROTH))
        return FORBIDDEN_REQUEST;
    
    if(S_ISDIR(PIGG_file_stat.st_mode))
        return BAD_REQUEST;
    
    int fd = open(PIGG_real_file, O_RDONLY);
    // 用来将某个文件内容映射到内存中，对该内存区域的存取即是直接对该文件内容的读写
    PIGG_file_address = (char*)mmap(0, PIGG_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return FILE_REQUEST;
}

// 读取的进程
//m_start_line是行在buffer中的起始位置，将该位置后面的数据赋给text
//此时从状态机已提前将一行的末尾字符\r\n变为\0\0，所以text可以直接取出完整的行进行解析
PIGG_http_conn::PIGG_HTTP_CODE PIGG_http_conn::process_read(){  
    //初始化从状态机状态、HTTP请求解析结果
    PIGG_LINE_STATUS line_status = LINE_OK;
    PIGG_HTTP_CODE ret = NO_REQUEST;
    char *text = 0;

    //这里为什么要写两个判断条件？第一个判断条件为什么这样写？
    //具体的在主状态机逻辑中会讲解。

    //parse_line为从状态机的具体实现
    while(PIGG_check_status == CHECK_STATE_CONTENT && 
    line_status == LINE_OK || ( (line_status = parse_line()) == LINE_OK) ){
        text = get_line();
        PIGG_start_line = PIGG_checked_idx;
        LOG_INFO("%s", text);
        switch(PIGG_check_status){
            case CHECK_STATE_REQUEST_LINE:{ //核对请求行
                ret = parse_request_line(text);
                if(ret == BAD_REQUEST)
                    return BAD_REQUEST;
                break;
            }
            case CHECK_STATE_HEADER:{   // 核对请求头部
                ret = parse_headers(text);
                if(ret == BAD_REQUEST)
                    return BAD_REQUEST;
                else if(ret == GET_REQUEST)
                    return do_request();
                break;
            }
            case CHECK_STATE_CONTENT:{
                ret = parse_content(text);
                if(ret == GET_REQUEST)
                    return do_request();
                line_status = LINE_OPEN;
                break;
            }
            default:
                return INTERNAL_ERROR;
        }
    }
    return NO_REQUEST;
}
