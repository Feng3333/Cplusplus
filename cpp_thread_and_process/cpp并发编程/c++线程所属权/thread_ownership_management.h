/* 
 * Description: c++ thread ownership management demo
 * Create: 2024-06-13
 * Owner: 
 */

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>

#ifndef THREAD_OWNERSHIP_MANAGEMENT
#define THREAD_OWNERSHIP_MANAGEMENT

namespace ThreadOwnershipManagement  {

void SomeFunction() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

void SomeOtherFunction() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};


/* 将一个线程的管理权交给一个已经绑定线程的变量，会触发线程的terminate函数引发崩溃 */
void DangerousUse() {
    // t1绑定SomeFunction
    std::cout << "t1绑定SomeFunction..." << std::endl;
    std::thread t1(SomeFunction);

    //2 转移t1管理的线程给t2，转移后t1无效
    std::cout << "转移t1管理的线程给t2..." << std::endl;
    std::thread t2 =  std::move(t1);

    //3 t1可继续绑定其他线程,执行SomeOtherFunction
    std::cout << "t1绑定其他线程..." << std::endl;
    t1 = std::thread(SomeOtherFunction);

    //4  创建一个线程变量t3
    std::cout << "创建一个线程变量t3..." << std::endl;
    std::thread t3;

    //5  转移t2管理的线程给t3
    std::cout << "转移t2管理的线程给t3..." << std::endl;
    t3 = std::move(t2);

    //6  转移t3管理的线程给t1
    std::cout << "转移t3管理的线程给t1..." << std::endl;
    t1 = std::move(t3);
    std::this_thread::sleep_for(std::chrono::seconds(2000));
    std::cout << "end..." << std::endl; 
};


/* 函数内部返回一个局部的std::thread变量 */
std::thread ReturnSomeFunctionThread() {
    return std::thread(SomeFunction);
};

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


/* joining_thread */
class JoiningThread {
    std::thread t_;

public: 
    // 如果一个函数声明为noexcept，那么它不应该抛出任何异常。如果该函数确实抛出了异常，程序会调用 std::terminate 终止程序
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


/* 容器存储 */
void UseVector() {
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 10; ++i) {
        threads.emplace_back(ParamFunction, i);
    }

    for (auto& t : threads) {
        t.join();
    }
}


/* 线程并发 */
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

void UseParallelAcc() {
    std::vector<int> vec(10000);
    for (int i = 0; i < 10000; ++i) {
        vec.push_back(i);
    }

    int sum = 0;
    sum = ParallelAccumualte<std::vector<int>::iterator, int> (vec.begin(), vec.end(), sum);

    std::cout << "1 + 2 + ..... + 9998 + 9999 = " << sum << std::endl;
}

} // namespace ThreadOwnershipManagement
#endif // THREAD_OWNERSHIP_MANAGEMENT