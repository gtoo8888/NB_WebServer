#include "PIGG_config.h"
#include <json/json.h>
#include <fstream>

bool PIGG_Config::get_json_config(){
    std::fstream ifs;
    ifs.open("/data_hdd/PIGG_WebServer/bin/configs/PIGG_webserver_config.json");
    if(!ifs.is_open()){
        printf("json is not exist\n");
        return false;
    }

    Json::Value root;
    Json::Reader read;
    if(!read.parse(ifs,root,false)){
        printf("read error\n");
        return false;
    }

    user = root["user"].asString();  
    passwd = root["passwd"].asString();  
    databasename = root["databasename"].asString();  
    port = root["port"].asInt();              
    log_write = root["log_write"].asBool();           
    trig_mode = root["trig_mode"].asBool();            
    listen_trig_mode = root["listen_trig_mode"].asBool();         
    conn_trig_mode = root["conn_trig_mode"].asBool();     
    opt_linger = root["opt_linger"].asBool();         
    sql_num = root["sql_num"].asUInt();            
    thread_num = root["thread_num"].asUInt();             
    close_log = root["close_log"].asBool();           
    log_record_max = root["log_record_max"].asUInt();    
    block_queue_max_len = root["block_queue_max_len"].asUInt();    
    block_queue_max_wait = root["block_queue_max_wait"].asUInt();    
    actor_model = root["actor_model"].asBool(); 
    log_queue = root["log_queue"].asBool();
    html_root = root["html_root"].asString();  
}

PIGG_Config::PIGG_Config(){
    get_json_config();
    // port = 8888;               //端口号,默认8888
    // log_write = 0;           //日志写入方式，默认同步
    // trig_mode = 0;            //触发组合模式,默认listenfd LT + connfd LT
    // listen_trig_mode = 0;         //listenfd触发模式，默认LT
    // conn_trig_mode = 0;       //connfd触发模式，默认LT
    // opt_linger = 0;          //优雅关闭连接，默认不使用
    // sql_num = 8;            //数据库连接池数量,默认8
    // thread_num = 1;             //线程池内的线程数量,默认8
    // close_log = 1;           //关闭日志,默认不关闭,1是开启
    // actor_model = 0;          //并发模型,默认是proactor
}


void usage(){
    std::cout << "Usage: PIGG_s -e/-d key [input-file] [output-file]" << std::endl;
}


void PIGG_Config::parse_arg(int argc,char *argv[]){
    if(argc <= 1)
    {
        usage();
    }else{
        // while(())
    }
}
