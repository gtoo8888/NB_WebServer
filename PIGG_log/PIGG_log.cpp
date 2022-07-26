#include "PIGG_log.h"

PIGG_log::PIGG_log(){
    PIGG_count = 0;
    PIGG_is_async = false;  // 初始化的时候，使用同步日志
}