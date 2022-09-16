#ifndef _SQL_CONNECTION_POOL_
#define _SQL_CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include "../PIGG_lock/PIGG_lock.h"

class PIGG_connection_pool{
public:
    MYSQL *GetConnection();               //获取数据库连接
    bool RealeaseConnection(MYSQL *conn); //释放连接
    int GetFreeConn();                    //获取连接
    void DestroyPool();                   //销毁所有连接

    //单例模式
    static PIGG_connection_pool *GetInstance(){
        static PIGG_connection_pool connPool;
        return &connPool;
    }

    void init(string url, string User, string PassWord, string DatabaseName,
              int Port, int MaxConn, int close_log);

private:
    PIGG_connection_pool();
    ~PIGG_connection_pool();

    int PIGG_MaxConn;  //最大连接数
    int PIGG_CurConn;  //当前已使用的连接数
    int PIGG_FreeConn; //当前空闲的连接数
    PIGG_locker lock;
    std::list<MYSQL *> connList; //连接池
    PIGG_sem reserve;

public:
    std::string PIGG_url;           //主机地址
    std::string PIGG_Port;          //数据库端口号
    std::string PIGG_User;          //登陆数据库用户名
    std::string PIGG_PassWord;      //登陆数据库密码
    std::string PIGG_Database_Name; //使用数据库名
    int PIGG_close_log;             //日志开关
}

class PIGG_connection_RALL{
public:
    PIGG_connection_RALL();
    ~PIGG_connection_RALL();

private:
    MYSQL *PIGG_conRAII;
    PIGG_connection_pool *poolRAII;
}
#endif