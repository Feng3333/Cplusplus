/* 
 * Description: c++ other locks description
 * Create: 2024-07-08
 * Owner: 
 */

#ifndef OTHER_LOCK
#define OTHER_LOCK

#include <iostream>
#include <mutex>
#include <thread>
#include <stack>
#include <map>
#include <shared_mutex>


namespace OtherLock {

/* unique_lock */
std::mutex mtx;
int sharedData = 0;

void UseUnqiueLock() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "lock success" << std::endl;
    sharedData++;
    lock.unlock();
};

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

// unique_lock延迟加锁
void DeferLock() {
    // 延迟加锁
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    // 可以加锁
    lock.lock();
    // 可以自动析构解锁, 也可以手动解锁
    lock.unlock();
};

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

/* 
 * 锁的粒度表示加锁的精细程度;
 * 一个锁的粒度要足够大，保证可以锁住要访问的共享数据;
 * 同时一个锁的粒度要足够小，保证非共享数据不被锁住影响性能;
*/
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


/* 共享锁 shared_mutex */
// c++17 提供shared_mutex
// c++14 提供shared_time_mutex
// c++11 需利用boost库
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


/* 递归锁 */
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

} // namespace OtherLock
#endif // OTHER_LOCK