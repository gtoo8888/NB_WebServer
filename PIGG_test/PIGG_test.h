#ifndef _PIGG_TEST_H_
#define _PIGG_TEST_H_


#include <iostream>
#include <memory>
#include <cstring>
#include <stdio.h>
#include <unistd.h>     // sleep(1),usleep() 微秒级
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
    void test_fflush();
    void test_cppversion();
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

void PIGG_test::test_fflush(){  // 测试强制刷新
    char buff[64];

    memset(buff, '\0',sizeof(buff));
    fprintf(stdout, "启用全缓冲");
    setvbuf(stdout, buff, _IOFBF,1024); //全缓冲、线缓冲、无缓冲

    fprintf(stdout, "这里是runoob.com\n");
    fprintf(stdout, "该输出将保存到buff\n");
    fflush(stdout);

    fprintf(stdout, "这将在编程时出现\n");
    fprintf(stdout, "最后休眠五秒钟\n");

    sleep(5);
}

void PIGG_test::test_cppversion(){
    printf("cpp version : %ld\n",__cplusplus);
}




}

#endif