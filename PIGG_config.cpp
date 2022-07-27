#include "PIGG_config.h"

PIGG_Config::PIGG_Config(){
    PORT = 9006;               //端口号,默认9006
    LOGWrite = 0;           //日志写入方式，默认同步
    TRIGMode = 0;            //触发组合模式,默认listenfd LT + connfd LT
    LISTENTrigmode = 0;         //listenfd触发模式，默认LT
    CONNTrigmode = 0;       //connfd触发模式，默认LT
    OPT_LINGER = 0;          //优雅关闭链接，默认不使用
    sql_num = 8;            //数据库连接池数量,默认8
    thread_num = 0;             //线程池内的线程数量,默认8
    close_log = 0;           //关闭日志,默认不关闭
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
