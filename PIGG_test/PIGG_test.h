#ifndef _PIGG_TEST_H_
#define _PIGG_TEST_H_


#include <iostream>
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
};

// PIGG_test::PIGG_test(){
//     ;
// }
// PIGG_test::~PIGG_test(){

// }

}

#endif