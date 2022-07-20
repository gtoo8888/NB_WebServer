#include "NB_webserver.h"


NB_WebServer::NB_WebServer(){

}

NB_WebServer::~NB_WebServer(){

}


void NB_WebServer::log_write(){
    if(m_close_log == 0){   // 是否关闭日志
        if(m_log_write == 1){   // 是否写日志
            // NB_Log::get_instance()->init
        }else{
            
        }

    }
}
void NB_WebServer::sql_pool(){

}
