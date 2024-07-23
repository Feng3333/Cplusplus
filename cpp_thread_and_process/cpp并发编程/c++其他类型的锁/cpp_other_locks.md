# C++ unique_lock,共享锁和递归锁

## 简介
本文介绍C++ 并发中使用的其他类型的锁，包括```unique_lock```，```shared_lock```, 以及```recursive_lock```等。```shared_lock```和```unique_lock```比较常用，而```recursive_lock```用的不多，或尽可能规避用这种锁。

## unique_lock
```unique_lock```和```lock_guard```基本用法相同，构造时默认加锁，析构时默认解锁，
但```unique_lock```有个好处就是可以手动解锁。这一点尤为重要，方便我们控制锁住区域的粒度(加锁的范围大小),也能支持和条件变量配套使用
```cpp
std::mutex mtx;
int sharedData = 0;

void UseUnqiueLock() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "lock success" << std::endl;
    sharedData++;
    lock.unlock();
};
```

可以通过```unique_lock```的```owns_lock```判断是否持有锁
```cpp
// 判断是否占有锁
void OwnsLock() {
    std::unique_lock<std::mutex> lock(mtx);
    sharedData++;
    if (lock.owns_lock()) {
        std::cout << "owns lock" << std::endl;
    } else {
        std::cout << "doesn't own lock !" << std::endl;
    }

    lock.unlock();
    if (lock.owns_lock()) {
        std::cout << "owns lock" << std::endl;
    } else {
        std::cout << "doesn't own lock !" << std::endl;
    }
};
```
```unique_lock```可以延迟加锁:
```cpp
void DeferLock() {
    // 延迟加锁
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    // 可以加锁
    lock.lock();
    // 可以自动析构解锁, 也可以手动解锁
    lock.unlock();
};
```

同时使用owns和defer
```cpp
// 同时使用owns和defer
void UseOwnDefer() {
    std::unique_lock<std::mutex> lock(mtx);
    // 判断是否拥有锁
    if (lock.owns_lock()) {
        std::cout << "Main thread has the lock." << std::endl;
    } else {
        std::cout << "Main thread does not has the lock." << std::endl;
    }

    std::thread t([](){
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
        if (lock.owns_lock()) {
            std::cout << "Thread has the lock." << std::endl;
        } else {
            std::cout << "Thread does not has the lock." << std::endl;
        }

        lock.lock();

        if (lock.owns_lock()) {
            std::cout << "Thread has the lock." << std::endl;
        } else {
            std::cout << "Thread does not has the lock." << std::endl;
        }

        lock.unlock();

    });

    t.join();
};
```
上述代码会依次输出, 但是程序会阻塞，因为子线程会卡在加锁的逻辑上，因为主线程未释放锁，而主线程又等待子线程退出，导致整个程序卡住。

和```lock_guard```一样，```unique_lock```也支持领养锁:
```cpp
// unique_lock支持领养锁
void UseOwnAdopt() {
    mtx.lock();
    std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);
    if (lock.owns_lock()) {
        std::cout << "owns lock" << std::endl;
    } else {
        std::cout << "does not have the lock" << std::endl;
    }
    lock.unlock();
};
```
尽管是领养的，但是打印还是会出现```owns lock```，因为不管如何锁被加上，就会输出```owns lock```。  
既然```unique_lock```支持领养操作也支持延迟加锁，那么可以用两种方式实现前文lock_guard实现的swap操作  
```cpp
// unique_lock锁的交换
int a = 9;
int b = 16;
std::mutex mtx1;
std::mutex mtx2;

void SafeSwap1() {
    std::lock(mtx1, mtx2);
    std::unique_lock<std::mutex> lock1(mtx1, std::adopt_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::adopt_lock);
    std::swap(a, b);
};

void SafeSwap2() {
    std::unique_lock<std::mutex> lock1(mtx1, std::adopt_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::adopt_lock);
    std::lock(lock1, lock2);
    std::swap(a, b);
};
```
注意一旦```mutex```被```unique_lock```管理，加锁和释放的操作就交给```unique_lock```，不能调用```mutex```加锁和解锁，因为锁的使用权已经交给```unique_lock```了。  
```mutex```是不支持移动和拷贝的，但是```unique_lock```支持移动，当一个```mutex```被转移给```unique_lock```后，可以通过```unique_ptr```转移其归属权.  
```cpp
//转移互斥量所有权
//互斥量本身不支持move操作，但是unique_lock支持
std::unique_lock<std::mutex> GetLock() {
    std::unique_lock<std::mutex> lock(mtx);
    sharedData++;
    return lock;
};

void UseReturn() {
    std::unique_lock<std::mutex> lock(GetLock());
    sharedData++;
};
```

锁的粒度表示加锁的精细程度，一个锁的粒度要足够大，保证可以锁住要访问的共享数据.同时一个锁的粒度要足够小，保证非共享数据不被锁住影响性能. 而```unique_ptr```则很好的支持手动解锁。
```cpp
// unique_lock支持手动解锁
void PrecisionLock() {
    std::unique_lock<std::mutex> lock(mtx);
    sharedData++;
    lock.unlock();
    // 不涉及共享数据的耗时操作不要放在锁内执行
    std::this_thread::sleep_for(std::chrono::seconds(1));
    lock.lock();
    sharedData++;
};
```

## 共享锁
试想这样一个场景，对于一个DNS服务，我们可以根据域名查询服务对应的ip地址，它很久才更新一次，比如新增记录，删除记录或者更新记录等。平时大部分时间都是提供给外部查询，对于查询操作，即使多个线程并发查询不加锁也不会有问题，但是当有线程修改DNS服务的ip记录或者增减记录时，其他线程不能查询，需等待修改完再查询。或者等待查询完，线程才能修改。也就是说读操作并不是互斥的，同一时间可以有多个线程同时读，但是写和读是互斥的，写与写是互斥的，简而言之，写操作需要独占锁。而读操作需要共享锁。  
要想使用共享锁，需使用共享互斥量```std::shared_mutex```,```std::shared_mutex```是C++17标准提出的。C++14标准可以使用```std::shared_time_mutex```,
1、```std::shared_mutex```
```
* 提供了 `lock()`, `try_lock()`, 和 `try_lock_for()` 以及 `try_lock_until()` 函数，这些函数都可以用于获取互斥锁。
* 提供了 `try_lock_shared()` 和 `lock_shared()` 函数，这些函数可以用于获取共享锁。
* 当 `std::shared_mutex` 被锁定后，其他尝试获取该锁的线程将会被阻塞，直到该锁被解锁。
```

2、```std::shared_timed_mutex```
```
* 与 `std::shared_mutex` 类似，也提供了 `lock()`, `try_lock()`, 和 `try_lock_for()` 以及 `try_lock_until()` 函数用于获取互斥锁。
* 与 `std::shared_mutex` 不同的是，它还提供了 `try_lock_shared()` 和 `lock_shared()` 函数用于获取共享锁，这些函数在尝试获取共享锁时具有超时机制。
* 当 `std::shared_timed_mutex` 被锁定后，其他尝试获取该锁的线程将会被阻塞，直到该锁被解锁，这与 `std::shared_mutex` 相同。然而，当尝试获取共享锁时，如果不能立即获得锁，`std::shared_timed_mutex` 会设置一个超时，超时过后如果仍然没有获取到锁，则操作将返回失败。
```

因此，```std::shared_timed_mutex``` 提供了额外的超时机制，这使得它在某些情况下更适合于需要处理超时的并发控制。然而，如果不需要超时机制，可以使用更简单的 ```std::shared_mutex```。  
C++11标准没有共享互斥量，可以使用```boost```提供的```boost::shared_mutex```。  
如果我们想构造共享锁，可以使用```std::shared_lock```，如果我们想构造独占锁, 可以使用```std::lock_gurad```.  
我们用一个类```DNService```代表DNS服务，查询操作使用共享锁，而写操作使用独占锁，可以是如下方式的:  
```cpp
class DNService {
public:
    DNService() {};
    // 读操作采用共享锁
    // QueryDNS用来查询dns信息，多个线程可同时访问
    std::string QueryDNS(std::string dnsName) {
        std::shared_lock<std::shared_mutex> sharedLocks(sharedMtx_);
        auto iter = dnsInfo_.find(dnsName);
        if (iter != dnsInfo_.end()) {
            return iter->second;
        }

        return "";
    };

    // 写操作采用独占锁
    // AddDNSInfo 用来添加dns信息，属独占锁，同一时刻只有一个线程在修改
    void AddDNSInfo(std::string dnsName, std::string dnsEntry) {
        std::lock_guard<std::shared_mutex> guardLocks(sharedMtx_);
        dnsInfo_.insert(std::make_pair(dnsName, dnsEntry));
    };

private:
    std::map<std::string, std::string> dnsInfo_;
    mutable std::shared_mutex sharedMtx_;
};
```

## 递归锁
有时候我们在实现接口的时候内部加锁，接口内部调用完结束自动解锁。会出现一个接口调用另一个接口的情况，如果用普通的std::mutex就会出现卡死，因为嵌套加锁导致卡死。但是我们可以使用递归锁。  
但我个人并不推荐递归锁，可以从设计源头规避嵌套加锁的情况，我们可以将接口相同的功能抽象出来，统一加锁。  
下面的设计演示了如何使用递归锁:
```cpp
class RecursiveDemo {
public:
    RecursiveDemo() {};
    bool QueryStudent(std::string name) {
        std::lock_guard<std::recursive_mutex> recursiveLock(recursiveMutex_);
        auto iter_find = studentsInfo_.find(name);
        if (iter_find == studentsInfo_.end()) {
            return false;
        }
        return true;
    };

    void AddScore(std::string name, int score) {
        std::lock_guard<std::recursive_mutex> recursiveLock(recursiveMutex_);
        if (!QueryStudent(name)) {
            studentsInfo_.insert(std::make_pair(name, score));
            return;
        }

        studentsInfo_[name] = studentsInfo_[name] + score;
    };


    // 不推荐采用递归锁，使用递归锁说明设计思路并不理想，需优化设计
    // 推荐拆分逻辑，将共有逻辑拆分为统一接口
    void AddScoreAtomic(std::string name, int score) {
        std::lock_guard<std::recursive_mutex> recursiveLock(recursiveMutex_);
        auto iter_find = studentsInfo_.find(name);
        if (iter_find == studentsInfo_.end()) {
            studentsInfo_.insert(std::make_pair(name, score));
            return;
        }

        studentsInfo_[name] = studentsInfo_[name] + score;
        return;
    };

private:
    std::map<std::string, int> studentsInfo_;
    std::recursive_mutex recursiveMutex_;
};
```
我们可以看到```AddScore```函数内部调用了```QueryStudent```, 所以采用了递归锁。
但是我们同样可以改变设计，将两者公有的部分抽离出来生成一个新的接口```AddScoreAtomic```.
```AddScoreAtomic```可以不使用递归锁，也能够完成线程安全操作的目的

