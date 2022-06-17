# std::atomic ?

## atomic介绍  

①：atomic 对 int、char、bool 等数据结构进行了原子性封装，在多线程环境中，对 std::atomic 对象的访问不会造成竞争-冒险。利用 std::atomic 可实现数据结构的无锁设计。  

②：所谓的原子操作，就是因为"原子是最小的，不可分割的最小个体的"的意义，它表示在多个线程访问同一个全局资源的时候，能够确保所有其他的线程不在同一时间内访问相同的资源。也就是它确保了在同一时刻只有唯一的线程对这个资源进行访问。(这个有点类似互斥对象对共享资源的访问的保护，但是原子操作更加接近底层，因而效率更高)。

③：在新标准 C++11 ，引入了原子操作的概念，并通过这个新的头文件 #include \<atomic> 提供了多种原子操作的数据类型，例如 atomic_bool , atomic_int 等等；如果在多个线程中对这些类型的共享资源进行操作，编译器就会保证这些操作都是原子性的，也就是说，确保任意时刻只有一个线程对这个共享资源进行访问，编译器将保证，多个线程访问这个共享资源的正确性，从而避免了锁的使用，提高的效率。

## atomic高效的体现（与加锁的情况进行比较）

### 加锁不使用atomic 

在win环境上可运行，但在linux上不行？？
```cpp
#include <iostream>
#include <ctime>
#include <mutex>
#include <vector>
#include <thread>

std::mutex mtx;
size_t count = 0;

void threadFun(){
  for (int i=0; i<10000; i++) {
    //防止多个线程同时访问同一个资源
    std::unique_lock<std::mutex> lock(mtx);
    count++;
  }
}

int main(void) {
  clock_t start_time = clock();
  
  //启动多个线程
  std::vector<std::thread> threads;
  for (int i=0; i<10; i++) {
    threads.push_back(std::thread(threadFun));
  }
  for (auto &thad : threads){
    thad.join();
  }
  
  //检测count是否正确： 10000*10 = 100000
  std::cout << "count number: " << count << std::endl;
  
  clock_t end_time = clock();
  std::cout << "耗时: " << end_time - start_time << "ms" << std::endl;
  
  return 0;
}

```
