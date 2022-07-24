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



    MYSQL *conn = NULL;
    conn = mysql_init(conn);

    int port = 3306;
    MYSQL_RES *res;
    MYSQL_ROW row;


    /* 连接数据库 */
    if (!mysql_real_connect(conn, "localhost", user.c_str(), passwd.c_str(), databasename.c_str(), port, NULL, 0)) {
        std::cout << mysql_error(conn);
        return -1;
    }
 
    /* 查询语句 */
    if (mysql_query(conn, "show databases")) {
        std::cout << mysql_error(conn);
        return -2;
    }
 
    res = mysql_use_result(conn);
 
    /* 打印查询结果 */
    while ((row = mysql_fetch_row(res)) != NULL) printf("%s \n", row[0]);
 
    /* 断开连接 */
    mysql_free_result(res);
    mysql_close(conn);



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