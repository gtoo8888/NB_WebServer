#include "PIGG_http.h"


const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form  = "Your request has bad sysntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form  = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form  = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form  = "There was an uinusual problem serving the requeset file.\n";


// 添加响应
bool PIGG_http_conn::add_response(const char * format, ...){
    if (PIGG_write_idx >= WRITE_BUFFER_SIZE)
        return false;
    va_list arg_list;       // 操作可变参数列表
    va_start(arg_list,format);
    int len = vsnprintf(PIGG_write_buf + PIGG_write_idx,WRITE_BUFFER_SIZE - 1 - PIGG_write_idx,format,arg_list);
    if(len >= (WRITE_BUFFER_SIZE - 1 - PIGG_write_idx)){ // 超过长度了
        va_end(arg_list);
        return false;
    }
    PIGG_write_idx += len;
    va_end(arg_list);

    LOG_INFO("request:%s",PIGG_write_buf);

    return true;
}

// 添加状态行
bool PIGG_http_conn::add_status_line(int status,const char *title){
    return add_response("%s %d %s\r\n","HTTP/1.1", status, title);
}

// 添加头
bool PIGG_http_conn::add_headers(int content_len){
    return add_content_length(content_len) && add_linger() && add_blank_line();
}
bool PIGG_http_conn::add_content_length(int content_len){
    return add_response("Content-Length:%d\r\n",content_len);
}
bool PIGG_http_conn::add_linger(){
    return add_response("Connection:%s\r\n",(PIGG_linger == true) ? "keep-alive" : "close");
}
bool PIGG_http_conn::add_blank_line(){
    return add_response("%s","\r\n");
}

// 添加上下文类型
bool PIGG_http_conn::add_content_type(){
    return add_response("Content-Type:%s\r\n","text/html");
}

// 添加正文内容
bool PIGG_http_conn::add_content(const char *content){
    return add_response("%s", content);
}


// 读取进程
bool PIGG_http_conn::process_write(PIGG_HTTP_CODE read_ret){
    switch(read_ret){
        case INTERNAL_ERROR:{   // 内部错误
            add_status_line(500,error_500_title);
            add_headers(strlen(error_500_form));
            if(!add_content(error_500_form))
                return false;
            break;
        }
        case BAD_REQUEST:{  // 请求文件没找到
            add_status_line(404,error_404_title);
            add_headers(strlen(error_400_form));
            if(!add_content(error_400_form))
                return false;
            break;
        }
        case FORBIDDEN_REQUEST:{    // 禁止请求
            add_status_line(403,error_403_title);
            add_headers(strlen(error_403_form));
            if(!add_content(error_403_form))
                return false;
            break;
        }
        case FILE_REQUEST:{ // 文件请求
            add_status_line(200, ok_200_title);
            if(PIGG_file_stat.st_size != 0){
                add_headers(PIGG_file_stat.st_size);
                PIGG_iv[0].iov_base = PIGG_write_buf;
                PIGG_iv[0].iov_len = PIGG_write_idx;
                PIGG_iv[1].iov_base = PIGG_file_address;
                PIGG_iv[1].iov_len = PIGG_file_stat.st_size;
                PIGG_iv_count = 2;
                bytes_to_send = PIGG_write_idx + PIGG_file_stat.st_size;
                return true;
            }else{
                const char* ok_string = "<html><body></body></html>";
                add_headers(strlen(ok_string));
                if(!add_content(ok_string)){
                    return false;
                }
            }
        }
        default:
            return false;
    }
    PIGG_iv[0].iov_base = PIGG_write_buf;
    PIGG_iv[0].iov_len = PIGG_write_idx;
    PIGG_iv_count = 1;
    bytes_to_send = PIGG_write_idx;
    return true;
}



