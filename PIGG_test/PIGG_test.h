#ifndef _PIGG_TEST_H_
#define _PIGG_TEST_H_


#include <iostream>
#include "../PIGG_log/PIGG_log.h"
// 如果想要写的函数被主函数中使用，需要在.h的命名空间中定义，在.cpp中定义不会被获取到
namespace PIGG_init_test{ // 测试的命名空间

// int test3 = 10;
// void test_namespace1(){
//     std::cout << "test_namespace1()" << std::endl;
// }


class PIGG_test{
public:
    // PIGG_test();
    // ~PIGG_test();
    void test_shared_ptr();
    void test_system_common();
    void test_fflush();
    void test_cppversion();
    void test_log();
    void test_fopen();
    void test_stat();
    void test_snprintf();
    void test_char_get_val();
    void test_str_some();




    void test_pthread();
    void test_pthread2();
    void test_pthread3();
    void test_pthread4();



    void test_json1_read();
    void test_json2_write();
    void test_json3();
};

// PIGG_test::PIGG_test(){
//     ;
// }
// PIGG_test::~PIGG_test(){

// }

}

#endif