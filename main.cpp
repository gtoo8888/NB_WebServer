#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <memory> //shared_ptr
#include "PIGG_config.h"
#include "PIGG_log/PIGG_log.h"
#include "PIGG_test/PIGG_test.h"
#include "PIGG_lock/PIGG_lock.h"
#include <mysql/mysql.h>

#include "PIGG_config.h"
// #include <QDebug>
// #include <QObject>
// #include <QTimer>



void test_namespace(){  // 对命名空间使用的测试
    // PIGG_init_test::PIGG_test *NBPIGG_test;
    // NBPIGG_test->test_shared_ptr();
    // PIGG_init_test::test_namespace1();
    // std::cout << "PIGG_init_test::test3" << std::endl;

    // PIGG_init_test::PIGG_test::test_shared_ptr()    // 这样的用法是错误的，这是一个非静态对象，一定要用它的实例去调用它
    PIGG_init_test::PIGG_test main_PIGG_test;        // 普通的生成一个类的实例的调用方法     
    main_PIGG_test.test_shared_ptr();
    main_PIGG_test.test_log();

    // std::shared_ptr<PIGG_init_test::PIGG_test> main_test = std::make_shared<PIGG_init_test::PIGG_test>();   // 制造了一个智能指针调用方法
    // main_test->test_shared_ptr();        // 这样的使用方式不好调试
}

void test_mysql(){
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "yourdb";

    MYSQL *conn = NULL;
    conn = mysql_init(conn);

    int port = 3306;
    MYSQL_RES *res;
    MYSQL_ROW row;

    /* 连接数据库 */
    if (!mysql_real_connect(conn, "localhost", user.c_str(), passwd.c_str(), databasename.c_str(), port, NULL, 0)) {
        std::cout << mysql_error(conn);
        return ;
    }


    for(int i = 0;i < 100;i++) {
        char name[100] = "56";
        char password[100] = "TTT";
        std::string test;

        // std::ostringstream ostr,ostr2;
        // ostr << i+1;
        // std::string astr1 = ostr.str();
        // ostr2 << i;
        // std::string astr2 = ostr2.str();

        // test = astr1 + "&" +  astr2;

        // char *m_string = &test[0]; //存储请求头数据
        // int j = 0;
        // for ( ;m_string[j] != '&'; ++j)
        //     name[j - 5] = m_string[i];
        // name[j - 5] = '\0';

        // int j = 0;
        // for (; m_string[j] != '\0'; ++j)
        //     password[j] = m_string[j];
        // password[j] = '\0';

        char *sql_insert = (char *)malloc(sizeof(char) * 200);
        strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
        strcat(sql_insert, "'");
        strcat(sql_insert, name);
        strcat(sql_insert, "', '");
        strcat(sql_insert, password);
        strcat(sql_insert, "')");
        std::cout << sql_insert << std::endl;
        if(mysql_query(conn, sql_insert))
        {
            std::cout << mysql_error(conn);
            return ;
        }
    }


    /* 查询语句 */
    if (mysql_query(conn, "select * from user")) {
        std::cout << mysql_error(conn);
        return ;
    }
 
    res = mysql_use_result(conn);
 
    /* 打印查询结果 */
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s %s\n", row[0],row[1]);
    }
 
    /* 断开连接 */
    mysql_free_result(res);
    mysql_close(conn);
}

int main(int argc,char *argv[]){
    int port = 8888;
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "yourdb";

    std::cout << "-----PIGG_server start run------" << std::endl;
    // 解析命令行
    PIGG_Config config;
    config.parse_arg(argc,argv);

    // 参数初始化
    PIGG_WebServer server;
    server.init(config.port, user, passwd, databasename, 
    config.opt_linger,config.trig_mode,config.sql_num,config.thread_num,
        config.actor_model,config.close_log,true);

    std::cout << "-----PIGG_WebServer::init()------" << std::endl;
    server.log_write();
    std::cout << "-----PIGG_WebServer::log_write()------" << std::endl;

    PIGG_init_test::PIGG_test log_test;
    log_test.test_log();


    server.sql_pool();  //数据库连接池
    std::cout << "-----PIGG_WebServer::sql_pool()------" << std::endl;
    server.thread_pool();   //开启线程池
    std::cout << "-----PIGG_WebServer::thread_pool()------" << std::endl;
    server.init_trig_mod(0,0);    // 默认LT+LT,不选择优雅关闭
    std::cout << "-----PIGG_WebServer::init_trig_mod()------" << std::endl;
    server.event_listen();      // socket通讯的基本流程，开始建立通讯
    std::cout << "-----PIGG_WebServer::event_listen()------" << std::endl;
    std::cout << "-----enent_loop()start------" << std::endl;
    server.event_loop();           // 整个主程序在这里循环
    


    // test_namespace(); // 对命名空间使用的测试,正常应该注释掉
    // PIGG_init_test::PIGG_test main_PIGG_test;        // 普通的生成一个类的实例的调用方法     
    // main_PIGG_test.test_fflush();
    // qDebug() << QString("imagesNum:%1, ").arg(123)
    // test_mysql();
    return 0;
}

