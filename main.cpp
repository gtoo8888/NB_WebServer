#include <iostream>
#include <string>
#include "config.h"


int main(int argc,char *argv[]){
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "yourdb";

    std::cout << "-----NB_server start run------" << std::endl;

    Config config;
    config.parse_arg(argc,argv);


    NB_WebServer NB_server;
    // server.init()

    NB_server.log_write();  // 开启日志写
    return 0;
}