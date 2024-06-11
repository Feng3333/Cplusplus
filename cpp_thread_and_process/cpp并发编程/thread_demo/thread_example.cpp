#include "thread_exmaple.h"


int main()
{   
    std::cout << "Begin thread test : " << std::endl;

    std::string testStr = "abcdefg";

    // 通过()初始化并启动一个线程
    std::thread t1(threadWork1, testStr);
    // 主线程等待子线程退出
    t1.join();

    // t2_1当成一个函数对象, 返回一个std::thread类型的值, 函数的参数为一个函数指针,该函数指针返回值为backgroundTask, 参数为void
    // std::thread t2_1(backgroundTask());
    // t2_1.join();

    // 需要多加一层()
    std::thread t2((BackgroundTask()));
    t2.join();

    // 使用{}方式初始化
    std::thread t3{BackgroundTask()};
    t3.join();

    // lambda表达式
    std::thread t4([](std::string str) {
        std::cout << "t4, str : " << str << std::endl;
    }, testStr);
    t4.join();

    // // detach 注意事项
    // OOps();
    // //防止主线程退出过快，需要停顿一下，让子线程跑起来detach
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // join 用法
    UseJoin();

    // 捕获异常
    CatchException();
    
    // 自动守卫
    AutoGuard();

    // 使用DangerOops可能存在崩溃
    // DangerOops(100);
    // 提前显示转化
    SafeOops(100); 

    // 引用参数
    RefOops(50);

    std::cout << "Thread test finish..." << std::endl;
    return 0; 
}
