#include "PIGG_webserver.h"


PIGG_WebServer::PIGG_WebServer(){

}

PIGG_WebServer::~PIGG_WebServer(){

}


void PIGG_WebServer::log_write(){
    if(m_close_log == 0){   // 是否关闭日志
        if(m_log_write == 1){   // 是否写日志
            // PIGG_Log::get_instance()->init
        }else{
            
        }

    }
}
void PIGG_WebServer::sql_pool(){

}
