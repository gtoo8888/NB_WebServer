#include "sql_connection_pool.h"

PIGG_connection_pool::PIGG_connection_pool(){
    PIGG_CurConn = 0;
    PIGG_FreeConn = 0;
}

PIGG_connection_pool::~PIGG_connection_pool(){
    DestroyPool();
}

void PIGG_connection_pool::init(std::string url, std::string User, std::string PassWord, std::string DatabaseName,
                                int Port, int MaxConn, int close_log){    
    PIGG_url = url;
    PIGG_Port = Port;
    PIGG_User = User;
    PIGG_PassWord = PassWord;
    PIGG_Database_Name = DatabaseName;
    PIGG_close_log = close_log;

    for(int i = 0;i < MaxConn;i++){
        MYSQL* con = NULL;
        con = mysql_init(con);

        if(con == NULL){
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        con = mysql_real_connect(con,url.c_str(),User.c_str(),PassWord.c_str(),DatabaseName.c_str(),Port,NULL,0);

        if(con == NULL){
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        connList.push_back(con);
        ++PIGG_FreeConn;
    }
    reserve = PIGG_sem(PIGG_FreeConn);
    PIGG_MaxConn = PIGG_FreeConn;
}

//当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
MYSQL* PIGG_connection_pool::GetConnection(){
    MYSQL* con = NULL;
    if(connList.size() == 0)
        return NULL;
    reserve.wait();

    lock.PIGG_lock();   // 加锁

    con = connList.front();
    connList.pop_front();
    --PIGG_FreeConn;
    ++PIGG_CurConn;

    lock.PIGG_unlock(); // 解锁
    return con;
}

//释放当前使用的连接
bool PIGG_connection_pool::RealseConnection(MYSQL *con){
    if(con == NULL)
        return false;
    lock.PIGG_lock();   // 加锁
    connList.push_back(con);
    ++PIGG_FreeConn;
    --PIGG_CurConn;
    lock.PIGG_unlock(); // 解锁

    reserve.post();
    return true;
}

//销毁数据库连接池
void PIGG_connection_pool::DestroyPool(){
    lock.PIGG_lock();
    if(connList.size() > 0){
        std::list<MYSQL*>::iterator it;
        for(it == connList.begin();it != connList.end();++it){
            MYSQL *con = *it;
            mysql_close(con);
        }
        PIGG_CurConn = 0;
        PIGG_FreeConn = 0;
        connList.clear();
    }
    lock.PIGG_lock();
}

int PIGG_connection_pool::GetFreeConn(){
    return this->PIGG_FreeConn;
}

/*****************************************************PIGG_connection_RALL************************************/
PIGG_connection_RALL::PIGG_connection_RALL(MYSQL **SQL,PIGG_connection_pool *connPool){
    *SQL = connPool->GetConnection();

    PIGG_conRAII = *SQL;
    PIGG_poolRAII = connPool;
}

PIGG_connection_RALL::~PIGG_connection_RALL(){
    PIGG_poolRAII->RealseConnection(PIGG_conRAII);
}
