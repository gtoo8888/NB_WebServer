#include "sql_connection_pool.h"

PIGG_connection_pool::PIGG_connection_pool(){
    PIGG_CurConn = 0;
    PIGG_FreeConn = 0;
}

//构造初始化
void connection_pool::(string url, string User, string PassWord, string DatabaseName,
              int Port, int MaxConn, int close_log){    
    PIGG_url = url;
    PIGG_Port = Port;
    PIGG_User = User;
    PIGG_PassWord = PassWord;
    PIGG_Database_Name = DatabaseName;
    PIGG_close_log = clos_log;

}

