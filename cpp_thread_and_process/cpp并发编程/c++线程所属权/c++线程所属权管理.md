# C++ 线程管控
## 简介
C++线程管控，包括移交线程的归属权，线程并发数量控制以及获取线程id等基本操作。

## 线程所属权
线程可以通过detach在后台运行或者让开辟这个线程的父线程等待该线程完成。  
但每个线程都应该有其归属权，也就是归属给某个变量管理，比如:  
```cpp
void SomeFunction() {
    // do something
}
std::thread t1(SomeFunction);
```
t1是一个线程变量，管理一个线程，该线程执行SomeFunction()  
对于std::thread C++ 不允许其执行拷贝构造和拷贝赋值, 所以只能通过移动和局部变量返回的方式将线程变量管理的线程转移给其他变量管理。  
C++ 中类似的类型还有std::mutex, std::ifstream, std::unique_ptr。  
比如下面代码示例，就说明了线程归属权的转移方式    
```cpp
void SomeFunction() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

void SomeOtherFunction() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

//1 t1绑定SomeFunction
std::thread t1(SomeFunction); 
//2 转移t1管理的线程给t2，转移后t1无效
std::thread t2 =  std::move(t1);
//3 t1 可继续绑定其他线程,执行SomeOtherFunction
t1 = std::thread(SomeOtherFunction);
//4  创建一个线程变量t3
std::thread t3;
//5  转移t2管理的线程给t3
t3 = std::move(t2);
//6  转移t3管理的线程给t1
t1 = std::move(t3);
std::this_thread::sleep_for(std::chrono::seconds(2000));
``` 
上述的代码会引发崩溃，是因为步骤6造成的崩溃。  
让主函数睡眠2000秒，是为了告诉规避主函数退出引发崩溃的问题，因为如果线程不detach或者join，主线程退出时会引发崩溃，而这些线程没有join和detach，此处为了演示是因为步骤6引发的崩溃，所以让主线程睡眠2000秒暂时不退出，但是程序仍然会崩溃，说明是步骤6导致的崩溃。  
上述代码中将t2管理的线程交给t3，之后将t3管理的线程交给t1，此时t1管理线程运行着 SomeFunction   
步骤6导致崩溃的原因就是将t3管理的线程交给t1，而此时t1正在管理线程运行 SomeOtherFunction。  
由此可以得出一个结论，就是不要将一个线程的管理权交给一个已经绑定线程的变量，否则会触发线程的terminate函数引发崩溃。  


std::thread 和 std::unique_ptr一样，可以在函数内部返回一个局部的std::thread变量:  
```cpp
void ParamFunction(int a) {
    while(true) {
        std::cout << "param is : " << a << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

std::thread G() {
    std::thread t(ParamFunction, 33);
    return t;
};
```
C++ 在返回局部变量时，会优先寻找这个类的拷贝构造函数，如果没有就会使用这个类的移动构造函数  

## joining_thread
```cpp
class JoiningThread {
    std::thread t_;

public: 
    JoiningThread() noexcept = default; 

    template<typename Callable, typename ... Args>
    explicit JoiningThread(Callable&& func, Args&& ...args): 
        t_(std::forward<Callable>(func), std::forward<Args>(args)...){}

    explicit JoiningThread(std::thread t) noexcept: t_(std::move(t)){}

    JoiningThread(JoiningThread&& other) noexcept: t_(std::move(other.t_)){}

    JoiningThread& operator=(JoiningThread&& other) noexcept {
        // 如果当前线程可汇合，则汇合等待线程完成再赋值
        if (Joinable()) {
            Join();
        }
        
        t_ = std::move(other.t_);
        return *this;
    };

    JoiningThread& operator=(std::thread other) noexcept {
        // 如果当前线程可汇合，则汇合等待线程完成再赋值
        if (Joinable()) {
            Join();
        }
        
        t_ = std::move(other);
        return *this;
    };

    ~JoiningThread() noexcept {
        if (Joinable()) {
            Join();
        }
    };

    void Swap(JoiningThread& other) noexcept {
        t_.swap(other.t_);
    };

    std::thread::id GetId() const noexcept {
        return t_.get_id();
    };

    bool Joinable() const noexcept {
        return t_.joinable();
    };

    void Join() {
        t_.join();
    };

    void Detach() {
        t_.detach();
    };

    std::thread& AsThread() noexcept {
        return t_;
    };

    const std::thread& AsThread() const noexcept {
        return t_;
    };
};
```

使用方法代码示例:
```cpp
void UseJoinThread() {
    // 1.根据线程构造函数构造jioningthread
    JoiningThread j1([](int maxIndex) {
        for (int i = 0; i < maxIndex; ++i) {
            std::cout << "in thread id: " << std::this_thread::get_id() << " cur index is: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }, 10);

    // 2.根据thread构造jioningthread
    JoiningThread j2(std::thread([](int maxIndex) {
        for (int i = 0; i < maxIndex; ++i) {
            std::cout << "in thread id: " << std::this_thread::get_id() << " cur index is: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }, 10));

    // 3.根据thread构造j3
    JoiningThread j3(std::thread([](int maxIndex) {
        for (int i = 0; i < maxIndex; ++i) {
            std::cout << "in thread id: " << std::this_thread::get_id() << " cur index is: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }, 10));

    // 4.把j3赋值给j1, JionThread内部会等待j1汇合结束后再将j3赋值给j1
    j1 = std::move(j3);
}
```

## 容器存储
容器存储线程时，比如vector,如果用push_back操作势必会调用std::thread，这样会引发编译错误，因为std::thread没有拷贝构造函数.  
这时候可以采用emplace方式，直接根据线程构造函数需要的参数构造，这样就避免了调用thread的拷贝构造函数.
```cpp
void UseVector() {
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 10; ++i) {
        threads.emplace_back(ParamFunction, i);
    }

    for (auto& t : threads) {
        t.join();
    }
}
```

## 选择运行数量
借用C++标准库的std::thread::hardware_concurrency()函数，它的返回值是一个指标，表示程序在各次运行中可真正并发的线程数量.  
代码示例: 模拟实现一个并行计算的功能，计算容器内所有元素的和  
```cpp
template<typename Iterator, typename T>
struct AccumulateBlock
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T ParallelAccumualte(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    //若容器中元素个数为0需直接返回
    if (!length) {
        return init;  
    }

    // 计算最大开辟的线程数, 预估每个线程计算25个数据长度。
    unsigned long const minPerThread = 25;
    unsigned long const maxThreads = (length + minPerThread - 1) / minPerThread;

    // HardwareThreads为cpu的核数
    unsigned long const HardwareThreads = std::thread::hardware_concurrency();
    std::cout << "Number of CPU cores : " << HardwareThreads << std::endl;

    // 开辟的线程数小于等于cpu核数，这样才不会造成线程过多时间片切换开销
    unsigned long const numThreads = std::min(HardwareThreads != 0 ? HardwareThreads : 2, maxThreads);
    // 计算步长，根据步长移动迭代器然后开辟线程计算。
    unsigned long const blockSize = length / numThreads;

    std::vector<T> results(numThreads);
    // 初始化了线程数-1个大小的vector，因为主线程也参与计算，所以这里-1
    std::vector<std::thread> threads(numThreads - 1);
    Iterator blockStart = first;

    for (unsigned long i = 0; i < (numThreads - 1); ++i) {
        Iterator blockEnd = blockStart;
        // 移动步长
        std::advance(blockEnd, blockSize);
        // 开辟线程
        threads[i] = std::thread(AccumulateBlock<Iterator, T>(), blockStart, blockEnd, std::ref(results[i]));
        // 更新起始位置。
        blockStart = blockEnd;
    }

    // 主线程计算
    AccumulateBlock<Iterator, T>()(blockStart, last, results[numThreads - 1]);
    // 让所有线程join
    for (auto& t : threads) {
        t.join();
    }

    // 最后将所有并行的计算结果再次调用std的accumulate算出结果
    return std::accumulate(results.begin(), results.end(), init);

}
```


## 识别线程
识别线程就是获取线程id，可以根据线程id是否相同判断是否同一个线程。  
比如启动了一个线程,可以通过线程变量的get_id()获取线程id:
```cpp
std::thread t([]() {
    std::cout << "thread start " << std::endl;
});

t.get_id();
```

如果想在线程的运行函数中区分线程，或者判断哪些是主线程或者子线程，可以通过以下方式:  
```cpp
std::thread t([]() {
    std::cout << "in thread id " << std::this_thread::get_id() << std::endl;
    std::cout << "thread start "  << std::endl;
});
```