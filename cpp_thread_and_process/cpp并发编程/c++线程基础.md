# C++ 线程基础

## 线程发起
线程发起就是要启动一个线程, C++11标准统一了线程操作, 可以在定义一个线程变量后，该变量启动线程执行回调逻辑.  
代码示例:  
```cpp
    void thread_work1(std::string str) {
        std::cout << "str is : " << str << std::endl;
    }

    std::string  str = "abcdefg";
    // 通过()初始化并启动一个线程
    std::thread t1(thread_work1, str);
```

## 线程等待
当启动一个线程后, 线程可能没有立即执行, 如果在局部作用域启动了一个线程, 或者main函数中, 很可能子线程没有运行就被回收了, 回收时会调用线程的析构函数, 执行terminate操作. 所以为了防止主线程退出或者局部作用域结束导致子线程被析构的情况, 可以通过join, 让主线程等待子线程启动运行, 子线程运行结束后主线程再运行.  
代码示例:
```cpp
    void thread_work1(std::string str) {
        std::cout << "str is : " << str << std::endl;
    }
    std::string str = "abcdefg";

    // 通过()初始化并启动一个线程
    std::thread t1(thread_work1, str);
    // 主线程等待子线程退出
    t1.join();
```

## 仿函数作为参数
当我们用仿函数作为参数传递给线程时, 也可以达到启动线程执行某种操作的含义  
```cpp
class BackgroundTask {
public:
    // 重载()运算符，让类可以作为一个函数直接被执行, 第二个()表示该函数可以接受什么类型的参数
    void operator()() {
        std::cout << "background task called" << std::endl;
    };
};
```

如果采用如下方式启动函数，那一定会报错的.  
```cpp
// t2_1当成一个函数对象, 返回一个std::thread类型的值, 函数的参数为一个函数指针,该函数指针返回值为BackgroundTask, 参数为void
// std::thread t2_1(BackgroundTask());
// t2_1.join();
```
因为编译器会将t2当成一个函数对象, 返回一个std::thread类型的值, 函数的参数为一个函数指针,该函数指针返回值为BackgroundTask, 参数为void。可以理解为如下  
```cpp
"std::thread (*)(BackgroundTask (*)())"
```

修改方式:   
```cpp
    //可多加一层()
    std::thread t2((backgroundTask()));
    t2.join();

    // 使用{}方式初始化
    std::thread t3{backgroundTask()};
    t3.join();
```

## lambda表达式
lambda表达式也可以作为线程的参数传递给thread  
```cpp
    // lambda表达式
    std::thread t4([](std::string str) {
        std::cout << "t4, str : " << str << std::endl;
    }, testStr);
    t4.join();
```

## 线程detach
线程允许采用分离的方式在后台独自运行，C++ concurrent programing 书中称其为守护线程。
```cpp

struct func {
    int& _i;
    func(int & i): _i(i){}
    void operator()() {
        for (int i = 0; i < 3; i++) {
            _i = i;
            std::cout << "_i is " << _i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

void OOps() {
        int SomeLocalState = 0;
        func myfunc(SomeLocalState);
        std::thread functhread(myfunc);
        //隐患，访问局部变量，局部变量可能会随着}结束而回收或随着主线程退出而回收
        functhread.detach();    
}; 

// detach 注意事项
OOps();
//防止主线程退出过快，需要停顿一下，让子线程跑起来detach
std::this_thread::sleep_for(std::chrono::seconds(1));
```
上面的例子存在隐患，因为SomeLocalState是局部变量, 当OOps调用结束后局部变量SomeLocalState就可能被释放了，而线程还在detach后台运行，容易出现崩溃。
所以在线程中使用局部变量的时候可以采取几个措施解决局部变量的问题:  
- 通过智能指针传递参数，因为引用计数会随着赋值增加，可保证局部变量在使用期间不被释放，这也就是我们之前提到的伪闭包策略。
- 将局部变量的值作为参数传递，这么做需要局部变量有拷贝复制的功能，而且拷贝耗费空间和效率。
- 将线程运行的方式修改为join，这样能保证局部变量被释放前线程已经运行结束。但是这么做可能会影响运行逻辑  
比如下面的修改:  
```cpp
void UseJoin() {
    int SomeLocalState = 0;
    func myFunc(SomeLocalState);
    std::thread funcThread(myFunc);
    funcThread.join();
};

// join 用法
UseJoin();
```

## 异常处理
当我们启动一个线程后，如果主线程产生崩溃，会导致子线程也会异常退出，就是调用terminate，如果子线程在进行一些重要的操作比如将充值信息入库等，丢失这些信息是很危险的。所以常用的做法是捕获异常，并且在异常情况下保证子线程稳定运行结束后，主线程抛出异常结束运行。如下面的逻辑:  
```cpp
// 捕获异常
void CatchException() {
    int SomeLocalState = 0;
    func myFunc(SomeLocalState);
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
```
但是用这种方式编码，会显得臃肿，可以采用RAII技术，保证线程对象析构的时候等待线程运行结束，回收资源。也可以写一个简单的线程守卫:  
```cpp
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
```
```cpp
void AutoGuard() {
    int SomeLocalState = 0;
    Func MyFunc(SomeLocalState);
    std::thread t(MyFunc);
    ThreadGuard tG(t);
    // 本线程做的一些事情
    std::cout << "Auto guard finished..." << std::endl;
}
```

## 慎用隐式转换
C++中会有一些隐式转换，比如char* 转换为string等。这些隐式转换在线程的调用上可能会造成崩溃问题
```cpp
void PrintStr(int i, std::string const& str) {
    std::cout << "i is: " << i << " str is: " << str << std::endl;
};

void DangerOops(int someParam) {
    char buffer[1024];
    sprintf(buffer, "%i", someParam);
    // 在线程内部将char const* 转化为 string
    // 常量指针 char* const 指针本身不能变
    // 指针常量 const char* 指向的内容不能变
    std::thread t(PrintStr, 3, buffer);
    t.detach();
    std::cout << "Danger oops finished..." << std::endl; 
};
```
当定义一个线程变量thread t时，传递给这个线程的参数buffer会被保存到thread的成员变量中。  
而在线程对象t内部启动并运行线程时，参数才会被传递给调用函数print_str。  
而此时buffer可能随着}运行结束而释放了。  
改进的方式很简单，将参数传递给thread时显示转换为string就可以了，这样thread内部保存的是string类型。
```cpp
void SafeOops(int someParam) {
    char buffer[1024];
    sprintf(buffer, "%i", someParam);
    std::thread t(PrintStr, 3, std::string(buffer));
    t.detach();
    std::cout << "Safe oops finished..." << std::endl; 
}
```

## 引用参数
当线程要调用的回调函数参数为引用类型时，需要将参数显示转化为引用对象传递给线程的构造函数。  
如果采用如下调用会编译失败：
```cpp
void ChangeParam(int& param) {
    param++;
};

void RefOopsWrong(int someParam) {
    std::cout << "Before change, param is : " << someParam << std::endl;
    std::thread t1(ChangeParam, someParam);
    t1.join();
    std::cout << "After change, param is : " << someParam << std::endl; 
};
```
即使函数change_param的参数为int&类型，我们传递给t2的构造函数为some_param,也不会达到在change_param函数内部修改关联到外部some_param的效果。  
因为some_param在传递给thread的构造函数后会转变为右值保存，右值传递给一个左值引用会出问题，所以编译出了问题.  


```cpp
void RefOops(int someParam) {
    std::cout << "Before change, param is : " << someParam << std::endl;
    // 需要使用显示转换
    std::thread t2(ChangeParam, std::ref(someParam));
    t2.join();
    std::cout << "After change, param is : " << someParam << std::endl; 
};
```

## 绑定类成员函数
有时候需要绑定一个类的成员函数
```cpp
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
```

如果thread绑定的回调函数是普通函数，可以在函数前加&或者不加&，因为编译器默认将普通函数名作为函数地址，如下两种写法都正确  
```cpp
void theadWork1(std::string str) {
    std::cout << "str is " << str << std::endl;
}
std::string helloStr = "hello world!";
//两种方式都正确
std::thread t1(theadWork1, hellostr);
std::thread t2(&theadWork1, hellostr);
```
但是如果是绑定类的成员函数，必须添加&

## 使用move操作
有时候传递给线程的参数是独占的，所谓独占就是不支持拷贝赋值和构造，但是可以通过std::move的方式将参数的所有权转移给线程，如下:  
```cpp
void DealUnique(std::unique_ptr<int> p) {
    std::cout << "unique ptr data is " << *p << std::endl;
    (*p)++;
    std::cout << "after unique ptr data is " << *p << std::endl;
}
void MoveOops() {
    auto p = std::make_unique<int>(100);
    std::thread  t(DealUnique, std::move(p));
    t.join();
    //不能再使用p了，p已经被move废弃
   // std::cout << "after unique ptr data is " << *p << std::endl;
}
```
