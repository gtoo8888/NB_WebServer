#ifndef _PIGG_TEST_H_
#define _PIGG_TEST_H_


#include <iostream>
#include <memory>
// 如果想要写的函数被主函数中使用，需要在.h的命名空间中定义，在.cpp中定义不会被获取到
namespace PIGG_init_test{ // 测试的命名空间




int test3 = 10;
void test_namespace1(){
    std::cout << "test_namespace1()" << std::endl;
}


class PIGG_test{
public:
    PIGG_test();
    ~PIGG_test();
    void test_shared_ptr();
    void test_system_common();
};

PIGG_test::PIGG_test(){
    ;
}
PIGG_test::~PIGG_test(){

}

void PIGG_test::test_shared_ptr(){    // 智能指针测试
    std::cout << "test_shared_ptr" << std::endl;

    std::shared_ptr<int> s_ptr = std::make_shared<int>(8);
    std::cout << *s_ptr << std::endl;
    std::cout << s_ptr.get() << std::endl;
    std::cout << s_ptr.use_count() << std::endl;
    std::cout << s_ptr.unique() << std::endl;
}


void PIGG_test::test_system_common(){// 系统命令的测试
    std::cout << "test_system_common" << std::endl;

    std::string folderPath = "lalalatest"; 
    std::string command;
    command = "touch " + folderPath;  
    system(command.c_str());

}




}

#endif