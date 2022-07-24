#include <iostream>
#include <string>
#include "NB_config.h"
#include <mysql/mysql.h>

int main(int argc,char *argv[]){
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "yourdb";

    std::cout << "-----NB_server start run------" << std::endl;

    Config config;
    config.parse_arg(argc,argv);


    std::string folderPath = "lalalatest"; 

    std::string command;
    command = "touch " + folderPath;  
    system(command.c_str());


    // MYSQL *con = NULL;
    // con = mysql_init(con);

    // if(con == NULL){
    //     exit(1);
    // }
    // int port = 8000;
    // con = mysql_real_connect(con,"localhost",user.c_str(),passwd.c_str(),databasename.c_str(),port,NULL,0);

    // if(con == NULL){
    //     exit(1);
    // }


    // NB_WebServer NB_server;
    // server.init()

    // NB_server.log_write();  // 开启日志写
    return 0;
}