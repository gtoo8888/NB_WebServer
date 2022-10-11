#include "PIGG_test.h"
#include <memory>
#include <cstring>
#include <stdio.h>
#include <unistd.h>     // sleep(1),usleep() 微秒级

namespace PIGG_init_test{

// PIGG_test::PIGG_test(){

// }
// PIGG_test::~PIGG_test(){

// }



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

void PIGG_test::test_log(){
    LOG_DEBUG("LOG_DEBUG %s","test1");
    LOG_INFO("LOG_INFO %s", "test2");
    LOG_WARN("LOG_WARN %s", "test3");
    LOG_ERROR("LOG_ERROR %s", "test4");
}


// "r"	以“只读”方式打开文件。只允许读取，不允许写入。文件必须存在，否则打开失败。
// "w"	以“写入”方式打开文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么清空文件内容（相当于删除原文件，再创建一个新文件）。
// "a"	以“追加”方式打开文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么将写入的数据追加到文件的末尾（文件原有的内容保留）。
// "r+" 以“读写”方式打开文件。既可以读取也可以写入，也就是随意更新文件。文件必须存在，否则打开失败。
// "w+" 以“写入/更新”方式打开文件，相当于w和r+叠加的效果。既可以读取也可以写入，也就是随意更新文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么清空文件内容（相当于删除原文件，再创建一个新文件）。
// "a+" 以“追加/更新”方式打开文件，相当于a和r+叠加的效果。既可以读取也可以写入，也就是随意更新文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么将写入的数据追加到文件的末尾（文件原有的内容保留）。
void PIGG_test::test_fopen(){
    FILE *pFile;
    pFile=fopen("myfile.txt","w");  // 
    if(pFile!=NULL)
    {
        fputs("it's a fopen example",pFile);
        fclose(pFile);
    }
    
    // fopen不能有中间未创建的文件夹，需要检验文件路径是否存在
    FILE* PIGG_fp1 = fopen("./temp/ServerLog", "a");    // 如果中间文件夹temp没有创建，那么FILE为空，打开失败
    if(PIGG_fp1 == NULL){    // 如果为空就失败了
        printf("fopen(log_full_name; error!\n");
    }
    FILE* PIGG_fp2 = fopen(".ServerLog", "a");    // 直接指定一个文件，那就直接可以打开文件
    if(PIGG_fp2 == NULL){    
        printf("fopen(log_full_name; error!\n");
    }

}


}