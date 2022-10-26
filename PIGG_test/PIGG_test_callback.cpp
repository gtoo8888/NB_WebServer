#include "PIGG_test.h"
#include <functional> // function/bind

namespace PIGG_init_test{

void programA_FunA1() {printf("programA_FunA1 \n");}    // 普通函数
void programA_FunA2() {printf("callback programA_FunA2 \n");}    // 普通函数作为回调函数
void prgramb_funb1(void (*callback)()){
    printf("programB_FunB1 \n");
    callback(); // 使用回调函数
}

void PIGG_test::test_callback1(){
    programA_FunA1();

    prgramb_funb1(programA_FunA2);  // 普通函数作为回调函数
}

class programA{
public:
    void funa1() {printf("programA.funa1() \n");} // 普通函数

    static void funa2() {printf("static callback programA.funa2() \n");}    // 静态函数作为回调函数
};
class programB{
public:
    void funb1(void (*callback)()){
        printf("programB.funb1() \n");
        callback();
    }
};

// 但这种实现有一个很明显的缺点：static 函数不能访问非static 成员变量或函数，会严重限制回调函数可以实现的功能。
void PIGG_test::test_callback2(){
    programA pa;
    pa.funa1();

    programB pb;
    pb.funb1(programA::funa2);  // 静态函数作为回调函数
    pb.funb1(pa.funa2);
    // pb.funb1(pa.funa1); // 编译器直接报错，不能使用非静态函数
    // pb.funb1(programA::funa1);   // 编译器直接报错，不能使用非静态函数
}


class programA2{
public:
    void funa1() {printf("programA2.funa1() \n");} // 普通函数

    void funa2() {printf("programA2.funa2() \n");}    
};

class programB2{
public:
    void funb1(void (programA2::*callback)(), void *context){   // 类的非静态函数作为回调函数
        printf("programB2.funb1() \n");
        ((programA2 *)context->*callback)();
    }
};

void PIGG_test::test_callback3(){
    programA2 pa;
    pa.funa1();

    programB2 pb;
    pb.funb1(&programA2::funa2,&pa);  // 类的非静态函数作为回调函数
    // 也就我预先还要知道回调函数所属的类定义，当programB想独立封装时就不好用了。
}



class programA3{
public:
    void funa1() {printf("programA3.funa1() \n");} // 普通函数

    void funa2() {printf("programA3.funa2() \n");}    

    static void funa2Wrapper(void *context){    // 把非static的回调函数 包装为另一个static函数
        printf("programA3.funa2Wrapper() \n");
        ((programA3 *)context)->funa2();    // 此处调用的FunA2()是context的函数, 不是this->FunA2()
    }
};

//ProgramB中没有ProgramA的任何信息了，是一种更独立的实现方式
// FunB2()通过调用FunA2Wrapper()，实现间接的对FunA2()的调用。FunA2()可以访问和调用对类内的任何函数和变量。多了一个wrapper函数，也多了一些灵活性
class programB3{
public:
    void funb1(void (programA3::*callback)(),void *context){    // 直接使用
        printf("callback programB3.funb1() \n");
        ((programA3*)context->*callback)();
    }
    void funb2(void (*callback)(void *),void *context){ // 使用包装
        printf("callback programB3.funb2() \n");
        callback(context);
    }
};

// 上面借助wrapper函数实现回调，虽然很灵活，但是还是不够优秀，比如：
// 1）多了一个不是太有实际用处的wrapper函数。
// 2）wrapper中还要对传入的指针进行强制转换。
// 3）FunB2调用时，不但要指定wrapper函数的地址，还要传入PA的地址。
void PIGG_test::test_callback4(){
    programA3 pa;
    pa.funa1();

    programB3 pb;
    pb.funb1(&programA3::funa2,&pa);
    pb.funb2(programA3::funa2Wrapper,&pa);
}



class programA4{
public:
    void funa1() {printf("programA4.funa1() \n");} 
    void funa2() {printf("programA4.funa2() \n");}    
    static void funa3() {printf("programA4.funa3() \n");}
};

class programB4{
public:
    typedef std::function<void ()> callbackfun;
    void funb1(callbackfun callback){
        printf("callback programB4.funb1() \n");
        callback();
    }

    void funb2(std::function<void ()> callback){
        printf("callback programB4.funb2() \n");
        callback();
    }
};

void normFun() {printf("normFun() \n");}

// std::function是一种通用、多态的函数封装。std::function的实例可以对任何可以调用的目标实体进行存储、复制、和调用操作，
// 这些目标实体包括普通函数、Lambda表达式、函数指针、以及其它函数对象等
// std::bind()函数的意义就像它的函数名一样，是用来绑定函数调用的某些参数的
// std::funtion是定义函数类型(输入、输出)
// std::bind是绑定特定的函数（具体的要调用的函数）
void PIGG_test::test_callback5(){
    programA4 pa;
    pa.funa1();

    programB4 pb;
    pb.funb1(normFun);
    pb.funb1(programA4::funa3);
    pb.funb1(std::bind(&programA4::funa2,&pa));
}

}