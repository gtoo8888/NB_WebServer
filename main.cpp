#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sstream>
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

    printf("%ld\n",__cplusplus);

    for(int i = 0;i < 100;i++) {
        char name[100];
        char password[100];
        std::string test;


        std::ostringstream ostr,ostr2;
        ostr << i+1;
        std::string astr1 = ostr.str();
        ostr2 << i;
        std::string astr2 = ostr2.str();

        test = astr1 + "&" +  astr2;

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
            return -1;
        }
    }


    /* 查询语句 */
    if (mysql_query(conn, "select * from user")) {
        std::cout << mysql_error(conn);
        return -2;
    }
 
    res = mysql_use_result(conn);
 
    /* 打印查询结果 */
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s %s\n", row[0],row[1]);
    }
 
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



// void dataBase::user_insert(UserBody &body) {
//     char select_user[255];
//     char query[255];
//     MYSQL_RES *result;
//     sprintf(select_user, "select * from user where UserName='%s'", body.userName);
//     if (mysql_query(&m_sqlCon, select_user)||!(result = mysql_store_result(&m_sqlCon))) {
//         std::cout << "插入查询失败" << std::endl;
//         return;
//     }
//     if (mysql_num_rows(result)) {
//         std::cout << "用户已存在" << std::endl;
//         mysql_free_result(result);
//         return;
//     }
//     mysql_free_result(result);
//     sprintf(query, "insert into user(UserName,UserPass,ConnectionNum,UseFor) values('%s','%s',%d,%d)", body.userName, body.userPass, body.connection, body.type);
//     if (mysql_query(&m_sqlCon, query)) {
//         std::cout << "插入数据失败" << std::endl;
//         return;
//     }
//     std::cout << "插入数据成功,共插入：" <<mysql_affected_rows(&m_sqlCon)<<"行" << std::endl;
// }




// void insert(MYSQL* conn, int ID, char name[20], int age, float score)
// //插入数据
// {
// 	char str[64] = "INSERT INTO student VALUES( ";
// 	char buffer[128] = { 0 };
// 	char str2[4] = ",'";
// 	char str3[4] = "',";
// 	char str4[2] = ",";
// 	char str5[2] = ")";
// 	int len = sprintf_s(buffer, "%s%d%s%s%s%d%s%f%s", str, ID, str2, name, str3, age, str4, score, str5);
// 	mysql_query(&mysql, buffer);
// 	if (len < 0)
// 		cout << "存档失败！" << endl;
// 	if (len > 0)
// 		cout << "存档成功！" << endl;
// }