#include "PIGG_test.h"
#include "hiredis/hiredis.h"

redisContext* globalcontext = nullptr;

namespace PIGG_init_test{

redisContext* PIGG_test::test_redis_connect(){
    redisContext* context = redisConnect("127.0.0.1",6379);
    if(context != NULL && context->err){
        LOG_ERROR("error is:%d",context->errstr);
        redisFree(context);
        return nullptr;
    }
    return context;
}

void PIGG_test::test_redis_command(){
    redisContext* context = test_redis_connect();
    redisReply* reply = (redisReply*)redisCommand(context, "SET mykey2 test");
    if(reply == NULL){
        LOG_ERROR("Error: Failed to execute Redis command\n");
        redisFree(context);
        return;
    }
    for(int i = 0;i < 10;i++){
        std::string key_num = "key" + std::to_string(i);
        std::string num = std::to_string(i);
        std::string comman_str = "SET " + key_num + " " + num;
        const char* comman = comman_str.c_str();
        // char *comman 
        // reply = (redisReply*)redisCommand(context, "SET %s %s",key_num, num);
        reply = (redisReply*)redisCommand(context, comman);
        if(reply == NULL){
            LOG_ERROR("Error: Failed to execute Redis command\n");
            redisFree(context);
            return;
        }
    }

    const char *argv[] = {"SET", "key", "value"};
    const size_t argvlen[] = {3, 3, 5};
    reply = (redisReply*)redisCommandArgv(context, 3, argv, argvlen);


    
    freeReplyObject(reply);
    redisFree(context);
}

}