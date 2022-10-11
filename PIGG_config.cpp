#include "PIGG_config.h"

PIGG_Config::PIGG_Config(){
    port = 8888;               //端口号,默认8888
    log_write = 0;           //日志写入方式，默认同步
    trig_mode = 0;            //触发组合模式,默认listenfd LT + connfd LT
    listen_trig_mode = 0;         //listenfd触发模式，默认LT
    conn_trig_mode = 0;       //connfd触发模式，默认LT
    opt_linger = 0;          //优雅关闭链接，默认不使用
    sql_num = 8;            //数据库连接池数量,默认8
    thread_num = 1;             //线程池内的线程数量,默认8
    close_log = 1;           //关闭日志,默认不关闭,1是开启
    actor_model = 0;          //并发模型,默认是proactor
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
