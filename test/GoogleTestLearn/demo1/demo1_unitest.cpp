#include "demo1.h"
#include "gtest/gtest.h"

// 输出结果：
// [==========] Running 9 tests from 2 test suites.
// [----------] Global test environment set-up.
// [----------] 6 tests from FactorialTest
// [ RUN      ] FactorialTest.Negative
// [       OK ] FactorialTest.Negative (0 ms)
// [ RUN      ] FactorialTest.Zeor
// [       OK ] FactorialTest.Zeor (0 ms)
// [ RUN      ] FactorialTest.positive
// [       OK ] FactorialTest.positive (0 ms)
// [ RUN      ] FactorialTest.4
// [       OK ] FactorialTest.4 (0 ms)
// [ RUN      ] FactorialTest.5
// [       OK ] FactorialTest.5 (6 ms)
// [ RUN      ] FactorialTest.error6
// /data_hdd/PIGG_WebServer/test/GoogleTestLearn/demo1/demo1_unitest.cpp:54: Failure
// Expected equality of these values:
//   2
//   Factorial(1)
//     Which is: 1
// 
// [  FAILED  ] FactorialTest.error6 (0 ms)
// [----------] 6 tests from FactorialTest (6 ms total)
// 
// [----------] 3 tests from IsPrimeTest
// [ RUN      ] IsPrimeTest.Negative
// [       OK ] IsPrimeTest.Negative (0 ms)
// [ RUN      ] IsPrimeTest.Trivial
// [       OK ] IsPrimeTest.Trivial (0 ms)
// [ RUN      ] IsPrimeTest.Positive
// [       OK ] IsPrimeTest.Positive (0 ms)
// [----------] 3 tests from IsPrimeTest (0 ms total)
// 
// [----------] Global test environment tear-down
// [==========] 9 tests from 2 test suites ran. (6 ms total)
// [  PASSED  ] 8 tests.
// [  FAILED  ] 1 test, listed below:
// [  FAILED  ] FactorialTest.error6
// 
//  1 FAILED TEST



//    * {ASSERT|EXPECT}_EQ(v1, v2): Tests that v1 == v2
//    * {ASSERT|EXPECT}_NE(v1, v2): Tests that v1 != v2
//    * {ASSERT|EXPECT}_LT(v1, v2): Tests that v1 < v2
//    * {ASSERT|EXPECT}_LE(v1, v2): Tests that v1 <= v2
//    * {ASSERT|EXPECT}_GT(v1, v2): Tests that v1 > v2
//    * {ASSERT|EXPECT}_GE(v1, v2): Tests that v1 >= v2
// {ASSERT|EXPECT}_GE(v1, v2),一般左边写期待的结果，右边写函数返回的结果
namespace{  
// 这是一个test suite，一组测试用例
// 可以不断的写测试样例来填充这个单元
// suite1:test1: 对负数进行测试
TEST(FactorialTest, Negative){ 
    EXPECT_EQ(1,Factorial(-5)); // 1
    EXPECT_EQ(1,Factorial(-1)); // 1
    EXPECT_GT(Factorial(-0),0); // 1


    EXPECT_EQ(1,Factorial(-1)); // 1 == 1
    EXPECT_NE(2,Factorial(-1)); // 2 != 1
    EXPECT_LT(0,Factorial(-1)); // 1 < 1
    EXPECT_LE(0,Factorial(-1)); // 1 <= 1
    EXPECT_GT(3,Factorial(-1)); // 3 > 1
    EXPECT_GE(1,Factorial(-1)); // 1 >= 1
}

// suite1:test2: 对零进行测试
TEST(FactorialTest, Zeor) {EXPECT_EQ(1,Factorial(0));}

// suite1:test3: 对正数进行测试
TEST(FactorialTest, positive){
    EXPECT_EQ(1,Factorial(1));
    EXPECT_EQ(2,Factorial(2));
}

// suite1:test4: 测试名是否可以为数字
TEST(FactorialTest, 4){
    EXPECT_EQ(1,Factorial(1));
}

// suite1:test5: 增加测试时间
TEST(FactorialTest, 5){
    EXPECT_EQ(1,Factorial(1));
    for(int i = 0;i < 100000;i++){
        EXPECT_EQ(1,Factorial(1));
    }
}

// suite1:test6: 错误情况
TEST(FactorialTest, error6){
    EXPECT_EQ(2,Factorial(1));
}



TEST(IsPrimeTest, Negative){
    EXPECT_FALSE(IsPrime(-1));
    EXPECT_FALSE(IsPrime(-2));
    EXPECT_FALSE(IsPrime(INT32_MIN));
}

TEST(IsPrimeTest, Trivial){
    EXPECT_FALSE(IsPrime(0));
    EXPECT_FALSE(IsPrime(1));
    EXPECT_TRUE(IsPrime(2));
    EXPECT_TRUE(IsPrime(3));
    EXPECT_FALSE(IsPrime(INT32_MIN));
}

TEST(IsPrimeTest, Positive) {
    EXPECT_FALSE(IsPrime(4));
    EXPECT_TRUE(IsPrime(5));
    EXPECT_FALSE(IsPrime(6));
    EXPECT_TRUE(IsPrime(23));
}

}
// 写一个Google Test测试用例的步骤
// 1.有一个包含GTEST_API_的主函数，引入testing::InitGoogleTest
// 2.包含头文件gtest/gtest.h
// 3.使用TEST宏定义，第一个参数是test suite的名字，第二个参数是test case的名字，写自己的测试函数
// 4.在测试函数中使用EXPECT_EQ,EXPECT_NE,EXPECT_LT,EXPECT_LE,EXPECT_GT,EXPECT_GE
// 5.在主函数中调用RUN_ALL_TESTS()，返回值是0表示成功，非0表示失败
// 6.编译，链接gtest库，运行
