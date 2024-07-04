/* 
 * Description: c++ thread mutually exclusive and deadlock
 * Create: 2024-06-20
 * Owner: 
 */

#ifndef LOCK_MUTEX
#define LOCK_MUTEX

#include <iostream>
#include <mutex>
#include <thread>
#include <stack>


namespace LockMutex {

std::mutex mtx1;
int sharedData = 100;

/* 锁的使用 */
void UseLock() {
    while(true) {
        mtx1.lock();
        sharedData++;
        std::cout << "current thread is : " << std::this_thread::get_id() << std::endl;
        std::cout << "shared data is : " << sharedData << std::endl;
        mtx1.unlock();
        // 通过短暂的睡眠达到释放cpu时间片的效果
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
};

void TestLock() {
    std::thread t1(UseLock);
    std::thread t2([]() {
        mtx1.lock();
        sharedData--;
        std::cout << "current thread is : " << std::this_thread::get_id() << std::endl;
        std::cout << "shared data is : " << sharedData << std::endl;
        mtx1.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    });

    t1.join();
    t2.join();
};


/* lock_guard的使用 */
void UseLock2() {
    while(true) {
        std::lock_guard<std::mutex> lock(mtx1);
        sharedData++;
        std::cout << "current thread is : " << std::this_thread::get_id() << std::endl;
        std::cout << "shared data is : " << sharedData << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }  
}

void TestLock2() {
    std::thread t1(UseLock);
    std::thread t2([]() {
        {
            std::lock_guard<std::mutex> lockGurad(mtx1);
            sharedData--;
            std::cout << "current thread is : " << std::this_thread::get_id() << std::endl;
            std::cout << "shared data is : " << sharedData << std::endl;
        } // 加上{}局部作用域来自动解锁
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    });

    t1.join();
    t2.join();
};


/* 如何保证数据全 */
template<typename T>
class ThreadSafeStack1 {
public:
    ThreadSafeStack1() {};

    ThreadSafeStack1(const ThreadSafeStack1& other) {
        std::lock_guard<std::mutex> lock(other.m_);
        data_ = other.data_;
    };

    ThreadSafeStack1& operator=(const ThreadSafeStack1&) = delete;

    void Push(T newValue) {
        std::lock_guard<std::mutex> lock(m_);
        data_.push(std::move(newValue));
    };

    // 问题代码
    T Pop() {
        std::lock_guard<std::mutex> lock(m_);
        auto element = data_.top();
        data_.pop();
        return element;
    };
    
    // 危险
    bool Empty() const {
        std::lock_guard<std::mutex> lock(m_);
        return data_.empty();
    };

private:
    std::stack<T> data_;
    mutable std::mutex m_; // 被mutable修饰的成员变量可以在常量成员函数中被修改，而不会导致编译错误
};

// 线程1和线程2先后判断栈都不为空，之后执行出战操作，会造成崩溃
void TestThreadSafeStack1() {
    ThreadSafeStack1<int> safeStack;
    safeStack.Push(1);

    std::thread t1([&safeStack]() {
        if (!safeStack.Empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            safeStack.Pop();
        }
    });

    std::thread t2([&safeStack]() {
        if (!safeStack.Empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            safeStack.Pop();
        }
    });

    t1.join();
    t2.join();
};

struct EmptyStack : std::exception
{
    const char* what() const throw();
};

template<typename T>
class ThreadSafeStack {
public:
    ThreadSafeStack() {};

    ThreadSafeStack(const ThreadSafeStack& other) {
        std::lock_guard<std::mutex> lock(other.m_);
        // 在构造函数的函数体(constructor body) 内进行复制操作
        data_ = other.data_;
    };

    ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

    void Push(T newValue) {
        std::lock_guard<std::mutex> lock(m_);
        data_.push(std::move(newValue));
    };

    std::shared_ptr<T> Pop() {
        std::lock_guard<std::mutex> lock(m_);
        // 弹出前检查是否为空栈
        if (data_.empty()) {
            throw EmptyStack();
            // return nullptr; 
        }
        // 改动栈容器前设置返回值
        std::shared_ptr<T> const res(std::make_shared<T>(data_.top()));
        data_.pop();
        return res;
    };

    void Pop(T& value) {
        std::lock_guard<std::mutex> lock(m_);
        if (data_.empty()) {
            throw EmptyStack();
        }
        value = data_.top();
        data_.pop();
    }

    bool Empty() const {
        std::lock_guard<std::mutex> lock(m_);
        return data_.empty();
    }

private:
    std::stack<T> data_;
    mutable std::mutex m_; // 被mutable修饰的成员变量可以在常量成员函数中被修改，而不会导致编译错误
};


/* 死锁是如何造成的 */
std::mutex tLock1;
std::mutex tLock2;
int m1 = 0;
int m2 = 0;

void DeadLock1 () {
    while (true) {
        std::cout << "DeadLock1 begin : " << std::endl;
        tLock1.lock();
        m1 = 1024;
        tLock2.lock();
        m2 = 2048;
        tLock2.unlock();
        tLock1.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        std::cout << "DeadLock1 end... " << std::endl;
    }
};

void DeadLock2 () {
    while (true) {
        std::cout << "DeadLock2 begin : " << std::endl;
        tLock2.lock();
        m2 = 2048;
        tLock1.lock();
        m1 = 1024;
        tLock1.unlock();
        tLock2.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        std::cout << "DeadLock2 end... " << std::endl;
    }
};

void TestDeadLock() {
    std::thread t1(DeadLock1);
    std::thread t2(DeadLock2);
    t1.join();
    t2.join();
};

// 加锁和解锁作为原子操作解耦合，各自只管理自己的功能
void AtomicLock1() {
    std::cout << "AtomicLock1 begin lock: " << std::endl;
    tLock1.lock();
    m1 = 1024;
    tLock1.unlock();
    std::cout << "AtomicLock1 end lock" << std::endl;
};

void AtomicLock2() {
    std::cout << "AtomicLock2 begin lock: " << std::endl;
    tLock2.lock();
    m2 = 2048;
    tLock2.unlock();
    std::cout << "AtomicLock2 end lock" << std::endl;
};

void SafeLock1() {
    while(true) {
        AtomicLock1();
        AtomicLock2();
        std::this_thread::sleep_for(std::chrono::microseconds(5));
    }
};

void SafeLock2() {
    while(true) {
        AtomicLock2();
        AtomicLock1();
        std::this_thread::sleep_for(std::chrono::microseconds(5));
    }
};

void TestSafeLock() {
    std::thread t1(SafeLock1);
    std::thread t2(SafeLock2);
    t1.join();
    t2.join();
};


/* 同时加锁 */
// 对于要使用两个互斥量,可以同时加锁, 如果不同时加锁，可能造成死锁
// 假设这是一个很复杂的数据结构，且不建议使用拷贝构造:
class SomeBigObject {
public:
    SomeBigObject(int data) : data_(data){};
    // 拷贝构造
    SomeBigObject(const SomeBigObject& b2): data_(b2.data_){};

    // 移动构造
    SomeBigObject(const SomeBigObject&& b2): data_(std::move(b2.data_)){};

    // 重载赋值运算符
    SomeBigObject& operator = (const SomeBigObject& b2) {
        if (this == &b2) {
            return *this;
        }
        
        data_ = b2.data_;
        return *this;
    }

    // 重载输出运算符
    friend std::ostream& operator << (std::ostream& os, const SomeBigObject& bigObject) {
        os << bigObject.data_;
        return os;
    };

    // 交换数据
    friend void Swap(SomeBigObject& b1, SomeBigObject& b2) {
        SomeBigObject temp = std::move(b1);
        b1 = std::move(b2);
        b2 = std::move(temp);
    };

private:
    int data_;
};

// 假设这是一个复杂的数据结构，包含了复杂的成员和一个互斥量
class BigObjectMgr {
public:
    BigObjectMgr(int data = 0) : obj_(data){};
    void PrintInfo() {
        std::cout << "current obj data is: " << obj_ << std::endl;
    };

    friend void DangerSwap(BigObjectMgr& objM1, BigObjectMgr& objM2);
    friend void SafeSwap(BigObjectMgr& objM1, BigObjectMgr& objM2);
    friend void SafeSwapScope(BigObjectMgr& objM1, BigObjectMgr& objM2);

private:
    std::mutex mtx_;
    SomeBigObject obj_;
};

void DangerSwap(BigObjectMgr& objM1, BigObjectMgr& objM2) {
    std::cout << "thread [" << std::this_thread::get_id() << "] begin" << std::endl;
    if (&objM1 == &objM2) {
        return;
    }
    
    std::lock_guard<std::mutex> guard1(objM1.mtx_);
    // 此处为了让死锁的情况必现
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> guard2(objM2.mtx_);
    Swap(objM1.obj_, objM2.obj_);
    std::cout << "thread [" << std::this_thread::get_id() <<"] end" << std::endl;
};


void SafeSwap(BigObjectMgr& objM1, BigObjectMgr& objM2) {
    std::cout << "thread [" << std::this_thread::get_id() << "] begin" << std::endl;
    if (&objM1 == &objM2) {
        return;
    }
    
    // 同时加锁
    std::lock(objM1.mtx_, objM2.mtx_);
    // 领养锁管理互斥量解锁
    std::lock_guard<std::mutex> guard1(objM1.mtx_, std::adopt_lock);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> guard2(objM2.mtx_, std::adopt_lock);
    Swap(objM1.obj_, objM2.obj_);
    std::cout << "thread [" << std::this_thread::get_id() <<"] end" << std::endl;
};

void SafeSwapScope(BigObjectMgr& objM1, BigObjectMgr& objM2) {
    std::cout << "thread [" << std::this_thread::get_id() << "] begin" << std::endl;
    if (&objM1 == &objM2) {
        return;
    }
    
    // c++17
    std::scoped_lock guard(objM1.mtx_, objM2.mtx_);
    Swap(objM1.obj_, objM2.obj_);
    std::cout << "thread [" << std::this_thread::get_id() <<"] end" << std::endl;   
};

void TestDangerSwap() {
    BigObjectMgr objM1(5);
    BigObjectMgr objM2(10);
    std::thread t1(DangerSwap, std::ref(objM1), std::ref(objM2));
    std::thread t2(DangerSwap, std::ref(objM2), std::ref(objM1));

    t1.join();
    t2.join();
    objM1.PrintInfo();
    objM2.PrintInfo();
};

void TestSafeSwap() {
    BigObjectMgr objM1(5);
    BigObjectMgr objM2(10);
    std::thread t1(SafeSwap, std::ref(objM1), std::ref(objM2));
    std::thread t2(SafeSwap, std::ref(objM2), std::ref(objM1));

    t1.join();
    t2.join();
    objM1.PrintInfo();
    objM2.PrintInfo();
};

void TestSafeSwapScope() {
    BigObjectMgr objM1(5);
    BigObjectMgr objM2(10);
    std::thread t1(SafeSwapScope, std::ref(objM1), std::ref(objM2));
    std::thread t2(SafeSwapScope, std::ref(objM2), std::ref(objM1));

    t1.join();
    t2.join();
    objM1.PrintInfo();
    objM2.PrintInfo();
};


/* 层级锁 */
class HierarchicalMutex {
public:
    explicit HierarchicalMutex(unsigned long value): hierarchyValue_(value), previousHierarchyValue_(0){};

    HierarchicalMutex(const HierarchicalMutex&) = delete;
    HierarchicalMutex& operator=(const HierarchicalMutex&) = delete;

    void Lock() {
        CheckForHierarchViolation();
        internalMutex_.lock();
        UpdateHierarchyValue();
    };

    void Unlock() {
        if (thisThreadHierarchyValue_ != hierarchyValue_) {
            throw std::logic_error("mutex hierarchy violated");
        }

        thisThreadHierarchyValue_ = previousHierarchyValue_;
        internalMutex_.unlock();
    };

    bool TryLock() {
        CheckForHierarchViolation();
        if (!internalMutex_.try_lock()) {
            return false;
        }

        UpdateHierarchyValue();
        return true;
    }

private:
    std::mutex internalMutex_;
    unsigned long const hierarchyValue_;  // 当前层级值
    unsigned long previousHierarchyValue_; // 上一次层级值
    static thread_local unsigned long thisThreadHierarchyValue_; // 本线程记录的层级值

    void CheckForHierarchViolation() {
        if (thisThreadHierarchyValue_ <= hierarchyValue_) {
            throw std::logic_error("mutex hierarchy violated");
        }
    };

    void UpdateHierarchyValue() {
        previousHierarchyValue_ = thisThreadHierarchyValue_;
        thisThreadHierarchyValue_ = hierarchyValue_;
    };

};
// 每个线程对thread_local变量的访问是隔离的，一个线程对thread_local变量的修改不会影响到其他线程
thread_local unsigned long HierarchicalMutex::thisThreadHierarchyValue_(ULONG_MAX);

void TestHierarchyLock() {
    HierarchicalMutex hmtx1(1000);
    HierarchicalMutex hmtx2(500);
    std::thread t1([&hmtx1, &hmtx2]() {
        hmtx1.Lock();
        hmtx2.Lock();
        hmtx2.Unlock();
        hmtx1.Unlock();
    });

    std::thread t2([&hmtx1, &hmtx2]() {
        hmtx2.Lock();
        hmtx1.Lock();
        hmtx1.Unlock();
        hmtx2.Unlock();
    });

    t1.join();
    t2.join();
};

} // namespace LockMutex

#endif // LOCK_MUTEX