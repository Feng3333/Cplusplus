#include <iostream>
#include <thread>
#include <string>


void threadWork1(std::string str) {
    std::cout << "thread_work1, str : " << str << std::endl;
};

class BackgroundTask {
public:
    // 重载()运算符，让类可以作为一个函数直接被执行, 第二个()表示该函数可以接受什么类型的参数
    void operator()() {
        std::cout << "background task called" << std::endl;
    };
};

struct Func {
    int& i_;
    Func(int & i): i_(i){}
    void operator()() {
        for (int i = 0; i < 3; i++) {
            i_ = i;
            std::cout << "i_ is " << i_ << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

// 线程detach
void OOps() {
        int SomeLocalState = 0;
        Func myFunc(SomeLocalState);
        std::thread functhread(myFunc);
        // 隐患，访问局部变量，局部变量可能会随着}结束而回收或随着主线程退出而回收
        functhread.detach();  // 子线程在后台自己运行
};

void UseJoin() {
    int SomeLocalState = 0;
    Func myFunc(SomeLocalState);
    std::thread funcThread(myFunc);
    funcThread.join();
};

// 捕获异常
void CatchException() {
    int SomeLocalState = 0;
    Func myFunc(SomeLocalState);
    std::thread funcThead{ myFunc };

    try {
        // 本线程做的事情, 可能引发崩溃
        std::this_thread::sleep_for(std::chrono::seconds());
    } catch (std::exception& e) {
        funcThead.join();
        throw;
    }

    funcThead.join();
};

class ThreadGuard {
public:

    explicit ThreadGuard(std::thread& t) : t_(t){};

    ~ThreadGuard() {
        // join只能调用一次
        if (t_.joinable()) {
            t_.join();        
        }
    };

    ThreadGuard(ThreadGuard const&) = delete;
    ThreadGuard& operator=(ThreadGuard const&) = delete;

private:
    std::thread& t_;
};

void AutoGuard() {
    int SomeLocalState = 0;
    Func MyFunc(SomeLocalState);
    std::thread t(MyFunc);
    ThreadGuard tG(t);
    // 本线程做的一些事情
    std::cout << "Auto guard finished..." << std::endl;
};


// 慎用隐式转换
void PrintStr(int i, std::string const& str) {
    std::cout << "i is: " << i << " str is: " << str << std::endl;
};

void DangerOops(int someParam) {
    char buffer[1024];
    sprintf(buffer, "%i", someParam);
    // 在线程内部将char const* 转化为 string
    // 指针常量 char* const 指针本身不能变
    // 常量指针 const char* 指向的内容不能变
    std::thread t(PrintStr, 3, buffer);
    t.detach();
    std::cout << "Danger oops finished..." << std::endl; 
};

void SafeOops(int someParam) {
    char buffer[1024];
    sprintf(buffer, "%i", someParam);
    // 提前显示装换为string类型
    std::thread t(PrintStr, 3, std::string(buffer));
    t.detach();
    std::cout << "Safe oops finished..." << std::endl; 
};


// 引用参数
void ChangeParam(int& param) {
    param++;
};

void RefOopsWrong(int someParam) {
    std::cout << "Before change, param is : " << someParam << std::endl;
    std::thread t1(ChangeParam, someParam);
    t1.join();
    std::cout << "After change, param is : " << someParam << std::endl; 
};

void RefOops(int someParam) {
    std::cout << "Before change, param is : " << someParam << std::endl;
    // 需要使用显示转换
    std::thread t2(ChangeParam, std::ref(someParam));
    t2.join();
    std::cout << "After change, param is : " << someParam << std::endl; 
};


// 绑定类成员函数
class X {
public:
    void DoLengthyWork() {
        std::cout << "do lengthy work..." << std::endl;
    };
private:
};

void BindClassOops() {
    X myX;
    std::thread t(&X::DoLengthyWork, &myX);
    t.join();
};