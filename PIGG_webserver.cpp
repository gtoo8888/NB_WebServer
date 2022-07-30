#include "PIGG_webserver.h"


// PIGG_WebServer::PIGG_WebServer(){

// }

// PIGG_WebServer::~PIGG_WebServer(){

// }

void PIGG_WebServer::init(int close_log,int log_queue){
    PIGG_close_log = close_log; // 是否关闭日志
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
void PIGG_WebServer::init_sql_pool(){

}

void PIGG_WebServer::init_trig_mod(){
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
