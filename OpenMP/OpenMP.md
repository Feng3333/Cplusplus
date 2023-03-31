# C++ OpenMP

## 目录
 - [基本介绍](#基本介绍)
 - [具体实现](#具体实现)
 - [OpenMP常用函数](#openmp常用函数)
 - [for循环并行化的基本用法](#for循环并行化的基本用法)
 - [数据的共享和私有化](#数据的共享和私有化)
 - [互斥所同步机制与事件同步机制](#互斥所同步机制与事件同步机制)
 - [线程的调度优化](#线程的调度优化)

## 基本介绍
头文件
```
#include <omp.h>
```
OpenMP:多线程文件操作  
OpenMP是由OpenMP Architecture Review Board牵头提出的，并已被广泛接受，用于共享内存并行系统的多处理器程序设计的一套指导性编译处理方案(Compiler Directive)  
OpenMP支持的编程语言包括C、C++和Fortran；而支持OpenMp的编译器包括Sun Compiler，GNU Compiler和Intel Compiler等。  

OpenMp提供了对并行算法的高层的抽象描述，程序员通过在源代码中加入专用的pragma来指明自己的意图，由此编译器可以自动将程序进行并行化，并在必要之处加入同步互斥以及通信。当选择忽略这些pragma，或者编译器不支持OpenMp时，程序又可退化为通常的程序(一般为串行)，代码仍然可以正常运作，只是不能利用多线程来加速程序执行。

## 具体实现
实例代码：
```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <omp.h>

int main() {
    std::vector<int> nums(1000000);
    iota(nums.begin(), nums.end(), 0);
    double start = omp_get_wtime(); 
    long long sum = 0;
#pragma omp parallel for num_threads(5) reduction(+:sum)
    for (size_t i = 0; i < nums.size(); ++i) {
        sum += nums[i];
    }
    double end = omp_get_wtime();
    std::cout << std::fixed << "time : " << float(end - start) << std::endl;
    std::cout << sum << std::endl;
    return 0;
}
```
上述代码需要编译后执行:
```
 g++ test.cpp -o test -fopenmp
 ./test
```
- 一般来说，通过预处理指示符 #pragma omp 来表示使用OpenMP。 例如上述代码中通过 #pragma omp parallel for 来指定接下来的第一个for循环采用多线程执行，此时编译器会根据
num_threads()来创建相应的线程数。未指明num_threads()时编译器默认根据CPU的个数来创建线程数。

## OpenMP常用函数
- 返回当前可用的处理器的个数
```c++
omp_get_num_procs() 
```
- 返回当前并行区域中活动线程的个数，如果在并行区域外部调用，返回 1
```c++
omp_get_num_threads()
```
- 返回当前的线程Id
```c++
omp_get_thread_num()
```
- 设置进入并行区域时，将要创建的线程个数
```c++
omp_set_num_threads()
```

## OpenMP的并行区域
```c++
#pragma omp parallel // 大括号内为并行区域
{
   ...
}
```

### for循环并行化的基本用法
#### 数据不相关性
利用OpenMP 实现for循环的并行化，需满足数据的不相关性。  
在循环并行化时，多个线程同时执行循环，迭代的顺序是不确定的。如果数据是非相关的，那么可以采用基本的(如下)
```c++
#pragma omp parallel for
```
预处理指示符。

如果语句 S1 和语句 S2 相关，那么必然存在以下两种情况之一：
- 1. 语句S1在一次迭代中访问存储单元L，而S2在随后的一次迭代中访问同一存储单元，则称之为循环迭代相关(loop carried dependence)；
- 2. S1和S2在同一循环迭代中访问同一存储单元L，但S1的执行在S2之前，则称之为非循环迭代相关(loop-independent dependence);

#### for循环并行化的几种声明方式
```c++
#include <iostream>
#include <omp.h>

int main() {
    // 声明形式一
    #pragma omp parallel 
    {
        #pragma omp for
        for (int i = 0; i < 10; ++i) {
            std::cout << i << std::endl;
        }
    }
    
    // 声明形式二
    #pragma omp parallel for
    for (int i = 0; i < 10; ++i) {
        std::cout << i << std::endl;
    }
    
    return 0;
}
```
上述代码中的两种声明形式是一样的，第二种声明形式更为简介，不过，第一种形式有一个好处：可以在并行区域内、for循环以外插入其他并行代码：
```cpp
// 声明形式一
#pragma omp parallel 
{
    std::cout << "OK." << std::endl;
    #pragma omp for
    for (int i = 0; i < 10; ++i) {
        std::cout << i << std::endl;
    }
}
```

#### for循环并行化的约束条件
尽管OpenMP可以很方便的对for循环进行并行化，但并不是所有的for循环都可以并行化。  
以下几种情形的for循环就无法使用OpenMP并行化：  
-  for循环的循环变量必须是有符号型，例如使用下述例子便无法编译通过
```c++
#pragma omp parallel for
for (unsigned int i = 0; i < 10; ++i) {
    ...
}
```
- for循环的比较操作符必须是: ==, <, <=, >, >= 。for中使用auto或者 比较操作符为 != 则会编译不通过
- for循环的增量必须是整数的加减，而且必须是一个循环不变量。例如，for (int i = 0; i < 10; i = i + 1) 编译不通过；(目前只支持i++, i--, ++i, --i)
- for循环的比较操作符如果是 <, <=, 那么循环变量只能增加。例如，for (int i = 0; i != 10; --i) 编译不通过；
- for循环必须是单入口，单出口 ==。循环内部不允许能够达到循环以外的跳出语句，exit除外。异常的处理也不必须在循环体内部处理。例如，如循环体内的break或者goto语句，会导致编译不通过。

```cpp
#include <iostream>
#include <omp.h>

int main() {
    int a[10] = {1};
    int b[10] = {2};
    int c[10] = {3};

#pragma omp parallel
{
    #pragma omp for
    for (size_t i = 0; i < 10; ++i) {
        c[i] = a[i] + b[i];
    }
}
    for (int i = 0; i < 10; ++i) {
        std::cout << c[i] << " \n"[i==9];
    }
    
    return 0;
}
```

####  嵌套for循环
```cpp
#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    std::vector<std::vector<int>> matrix(10, std::vector<int>(10));
    
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                matrix[i][j] = i + j;
            }
        }
    }
    
    return 0;
}
```
编译器会让第一个线程完成
```cpp
for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 10; ++j) {
         matrix[i][j] = i + j;
    }
}
```
编译器会让第二个线程完成
```cpp
for (int i = 5; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
         matrix[i][j] = i + j;
    }
}
```

## 数据的共享和私有化
- 在并行区域内，若多个线程共同访问同一个存储单元，并且至少会有一个线程更新数据单元中的内容时，会发生数据竞争；
#### 并行区域内的变量共享和私有
除了以下三种情况外，并行区域中的所有变量都是共享的：
- 并行区域中定义的变量；
- 多个线程用来完成循环的循环变量；
- private、firstprivate、lastprivate、reduction修饰的变量
例如：
```c++
#include <iostream>
#include <omp.h>

int main() {
    int share_a = 0; // 共享变量
    int share_to_private_b = 1;
    
#pragma omp parallel
{
    int private_c = 2;
    // 通过private修饰后在并行区域内变为私有变量
    #pragma omp for private(share_to_private_b)
    for (int i = 0; i < 10; ++i) {
        // 该循环变量是私有的，若为两个线程，则一个线程执行0 <= i < 5,另一个执行5 <= i < 10
        std::cout << i << std::endl;
    }
}
    
    return 0;
}
```

#### 共享与私有变量声明方法
- private(val1, val2, ...) : 并行区域中变量val是私有的，即每个线程拥有该变量的一个copy
- firstprivate(val1, val2, ...) : 与private不同，每个线程在开始的时候都会对该变量进行一次初始化
- lastpriavte(val1, val2, ...) : 与private不同，并发执行的最后一次循环的私有变量将会copy到val
- shared(val1, val2, ...) : 声明val是共享的

private示例：
```cpp
#include <iostream>
#include <omp.h>

int main() {
    // 通过private修饰该变量之后在并行区域变为私有变量，进入并行区域后每个线程拥有该变量的拷贝，并且都不会初始化
    int shared_to_private = 1;

#pragma omp parallel for private(shared_to_private)
    for (int i = 0; i < 10; ++i) {
        std::cout << shared_to_private << std::endl;
    }
    
    return 0;
}
```

firstprivate示例：
```cpp
#include <iostream>
#include <omp.h>

int main() {
    // 通过firstprivate修饰该变量之后在并行区域变为私有变量，进入并行区域后每个线程拥有该变量的拷贝，并且会初始化
    int share_to_first_private = 1;

#pragma omp parallel for firstprivate(share_to_first_private)
    for (int i = 0; i < 10; ++i) {
        std::cout << ++share_to_first_private << std::endl;
    }
    
    return 0;
}
```

lastprivate示例：
```cpp
#include <iostream>
#include <omp.h>

int main() {
    // 通过lastprivate修饰该变量之后在并行区域变为私有变量，进入并行区域后变为私有变量，进入并行区域后每个线程拥有该变量的拷贝，并且会初始化
    int share_to_last_private = 1;
    
    std::cout << "Before: " << share_to_last_private << std::endl;
#pragma omp parallel for firstprivate(share_to_last_private)
    for (int i = 0; i < 11; ++i) {
        std::cout << ++share_to_last_private << std::endl;
    }
    
    std::cout << "After: " << share_to_last_private << std::endl;
    return 0;
}
```
在运行完后，share_to_last_private变量的值变了，其值会后变成最后一次循环的值，即多个线程最后一次修改的share_to_last_private(是share_to_last_private的copy)
值会赋给share_to_last_private。

shared示例：
```cpp
#include <iostream>
#include <omp.h>

int main() {
    int sum = 0;
    std::cout << "Before: " << sum << std::endl;
#pragma omp parallel for shared(sum)
    for (int i = 0; i < 10; ++i) {
        sum += i;
        std::cout << sum << std::endl;
    }
    
    std::cout << "After: " << sum << std::endl;
    return 0;
}
```
在上面的代码中，sum本身是共享的，这里的shared的声明作为演示用。由于sum是共享的，多个线程对sum的操作会引起数据竞争

#### reduction的用法：
```cpp
#include <iostream>
#include <omp.h>

int main() {
    int sum = 0;
    std::cout << "Before: " << sum << std::endl;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 10; ++i) {
        sum += i;
        std::cout << sum << std::endl;
    }
    
    std::cout << "After: " << sum << std::endl;
    return 0;
}
```
上述代码中：sum是共享的，采用reduction之后，每个线程根据reduction(+:sum)的声明算出自己的sum，然后再将每个线程的sum加起来。

reduction声明可以看做：
- 保证了对sum的原则操作
- 多个线程的执行结果通过reduction中声明的操作符进行计算，以加法操作符为例：
  假设sum的初始化的值为10，reduction(+:sum)声明的并行区域中每个线程的sum初始化为0，并行处理结束之后，会将sum的初始值10以及每个线程所计算的sum值相加。
- reduction声明形式如下：
```c++
reduction(opreator: val1, val2, ...)
```
其中operator以及约定变量的初始值如下：  
![image](https://github.com/Feng3333/Cplusplus/blob/f2f9792bef1e50479c34f7a8d18715f2cbb84cc9/images-folder/OMP_reduction.PNG)

## 互斥所同步机制与事件同步机制
#### 互斥锁同步
互斥锁同步的概念类似于Windows中的临界区（Critical Section）以及Windows和Linux中的Mutex以及Vxworks中的SemTake和SemGive（初始化时信号量为满）
，即对某一块代码操作进行保护，以保证同时只能有一个线程执行该段代码

#### atomic 同步用法
```
#pragma omp atomic
x< + or * or - or * or / or & or | or << pr >> >=expt
(例如，x <<= 1; or x *= 2)

#pragma omp atomic
x++ or x-- or --x or ++x
```

可以看到atomic的操作仅适用于两种情况：
- 自加减操作；
- x<上述列出的操作符>=expr

代码示例：
```c++
#include <iostream>
#include <omp.h>

int main() {
    int sum = 0;
    std::cout << "Before: " << sum << std::endl;
#pragma omp parallel for 
    for (int i = 0; i < 2000; ++i) {
    #pragma omp atomic
        sum++;
    }
    
    std::cout << "After: " << sum << std::endl;
    return 0;
}
```
输出结果为：2000，如果将#pragma omp atomic声明去掉，则结果不确定

#### cirtical同步机制
Cirtical Section 声明方法
```c++
#pragma omp critical [(name)]  // []表示名字可选
{
    // 并行程序块，同时只能有一个线程能访问该并行程序块
}

// 举例：
#pragma omp critial (tst)
a = b + c;
```
- cirtical section 与 atomic 的区别：临界区critical 可以对某个并行程度块进行保护，atomic所能保护的仅为一句代码。
示例：
```c++
#include <iostream>
#include <omp.h>

int main() {
    int sum = 0;
    std::cout << "Before: " << sum << std::endl;
    
 #pragma omp parallel for
     for (int i = 0; i < 10; ++i) {
     #pragma omp critial (a)
         {
             sum = sum + i;
             sum = sum + i * 2;
         }
     }
     
     std::cout << "After: " << sum << std::endl;
     return 0;
}
```

#### 线程同步只互斥锁函数
互斥锁函数类似于Windows、Linux下的mutex
- 互斥锁函数:
```c++
void omp_init_lock(omp_lock*)       // 初始化互斥锁
void omp_destroy_lock(omp_lock*)    // 销毁互斥锁
void omp_set_lock(omp_lock*)        // 获得互斥锁
void omp_unset_lock(omp_lock*)      // 释放互斥锁
void omp_test_lock(omp_lock*)       // 试图获得互斥锁，如果获得成功则返回true，否则返回false
```

- 示例：
```c++
#include <iostream>
#include <omp.h>

static omp_lock_t lock;

int main() {
    omp_init_lock(&lock);   // 初始化互斥锁

#pragma omp parallel for 
    for (int i = 0; i < 5; ++i) {
        omp_set_lock(&lock);  // 获得互斥锁
        std::cout << omp_get_thread_num() << "+" << std::endl;
        std::cout << omp_get_thread_num() << "-" << std::endl;
        omp_unset_lock(&lock); // 释放互斥锁
    }

    omp_destroy_lock(&lock);  // 销毁互斥锁

    return 0;
}
```
上述示例代码中对for循环中的所有内容进行加锁保护，同时只能有一个线程执行for循环中的内容。  
线程1或线程2在执行for循环内部代码时不会被打断。如果删除代码中的获得锁释放锁的代码，则相当于没有互斥锁。  
互斥锁函数中只有omp_test_lock()函数是带有返回值的，该函数可以看做是omp_set_lock的非阻塞版本

#### 线程同步之事件同步机制
线程同步机制包括互斥锁同步和事件同步。互斥锁同步包括：atomic、critical、mutex函数，其机制与普通多线程同步的机制类似。而事件同步则通过nowait、section、
single、master等预处理指示符声明来完成

- 隐式栅障
栅障(baarrier) 是OpenMP用于线程同步的一种方法，线程遇到栅障时必须等待，直到并行的所有线程都到达同一点。
需要注意的是：  
在任务分配for循环和任务分配section结构中隐含了栅障，在parallel，for，sections，single结构的最后，也会有一个隐式的栅障。  
隐式的栅障会使线程等到所有的线程继续完成当前的循环、结构化块或并行区，在继续执行后续工作。可以使用nowait去掉这个隐式的栅障。

- nowait事件同步
nowait用来取消栅障，其用法如下：
```c++
#pramga omp for nowait   // 不能使用#pramga omp parallel for nowait
// 或者
#pramga omp single nowait
```

示例：
```c++
#include <iostream>
#include <omp.h>

int main() {
#pragma omp parallel
{
    #pragma omp for nowait
    for (int i = 0; i < 1000; ++i) {
        std::cout << i << " + " << std::endl;
    }
    
    #pragma omp for
    for (int j = 0; j < 10; ++j) {
        std::cout << j << " - " << std::endl;
    }
}

    return 0;
}
```
运行上述程序时，可以看到第一个for循环的两个线程中的一个执行完之后，会继续向下执行，因此同时打印了第一个循环 + 和第二个循环的 - 。  
如果去掉第一个循环里的nowait声明，则第一个for循环的两个线程都执行完之后，才开始同时执行第二个for循环。也就是说，通过#pragma omp for 声明的for
循环结束时会有一个默认的隐式栅障。

- 显示同步栅障 #pragma omp barrier
```c++
#include <iostream>
#include <omp.h>

int main() {
#pragma omp parallel
{
    for (int i = 0; i < 100; ++i) {
        std::cout << i << " + " << std::endl;
    }
    
    #pragma omp barrier
    for (int j = 0; j < 10; ++j) {
        std::cout << j << " - " << std::endl;
    }
}

    return 0;
}
```
运行上述程序，可以看出两个线程执行了第一个for循环，当两个线程同时执行完第一个for循环之后，在barrier处进行了同步，然后执行后边的for循环

- master事件同步
通过# pragma omp master 来声明对应的并行程序块只有主线程完成
```c++
#include <iostream>
#include <omp.h>

int main() {
#pragma omp parallel
{
    #pragma omp master
    {
        for (int i = 0; i < 10; ++i) {
            std::cout << i << " + " << std::endl;
        }
    }

    std::cout << "This will printed twice." << std::endl;
}
    return 0;
}
```
运行上述程序，可以看到进入parallel声明的并行区域后，创建了两个线程。主线程执行了for循环；而另一个线程没有执行for循环，并且直接开始进入了for循环之后的打印语句，然后执行for循环
的线程随后还会在执行一次后边的打印语句

- sections 指定不同的线程执行不同的部分
示例：
```c++
#include <iostream>
#include <omp.h>

int main() {
// 声明该并行区域分为若干个section，section之间的运行顺序为并行
#pragma omp parallel sections
{
    for (int i = 0; i < 5; ++i) {
        std::cout << i << " + " << std::endl;
    }

#pragma omp section
    for (int j = 0; j < 5; ++j) {
        std::cout << j << " - " << std::endl;
    }
}
    return 0;
}
```

## 线程的调度优化
由omp生成的并行区域，在默认情况下会自动生成与CPU个数相等的线程，然后并行执行并行区域的代码。  
对于并行区域中的for循环有特殊的声明方式，这样不同的线程可以分别运行for循环变量的不同部分。通过锁同步(atomic、critical、mutex函数)或事件同步
(nowait、single、section、master)来实现并行区域的同步控制。  

### 调度策略
- static： 循环变量区域分为n等份，每个线程平分n份任务；  适用于各个cpu性能差异不大的场景
- dynamic：  循环变量区域分为n等份，某个线程执行完1份之后执行其他需要执行的那份任务；  适用于cpu之间运行能力差异较大的场景
- guided：  循环变量区域由大到小分为不等的n份，运行方法与dynamic类似；  由于任务比dynamic不同，所以可以减小调度开销
- runtime： 在运行时来适用上述三种调度策略中的一种，默认使用static

### 简单的代码示例：

- static
```c++
#include <iostream>
#include <omp.h>

int main() {

// static调度策略，for循环每两次迭代分为一个任务
#pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < 10; ++i) {
        std::cout << " thread id is " << omp_get_thread_num() << " num i : " << i << std::endl;
    }

    return 0;
}
```

- dynamic
```c++
#include <iostream>
#include <omp.h>

int main() {

#pragma omp parallel for schedule(dynamic, 2)
    for (int i = 0; i < 10; ++i) {
        // dynamic调度策略，分为5个任务，只要有任务并且线程空闲，那么该线程会执行该任务
        std::cout << " thread id is " << omp_get_thread_num() << " num i : " << i << std::endl;
    }

    return 0;
}
```

- guided
guided调度策略与dynamic区别在于：所分的任务块是从大到小排列的。具体分块算法为：每块的任务大小为【迭代次数 / 线程个数的2倍】。
其中每个任务的最小迭代次数有guided声明设定，默认为1。
```c++
#pragma omp for schedule [guided, 80]
    for (int i = 0; i < 800; ++i) {
        ...
    }
```
如果有两个cpu。那么任务分配如下：
```
第一个任务： [800 / (2 * 2)] = 800;

第二个任务： 第一个任务分了200 ，还有600 [600 / (2 * 2) ] = 150;

第三个任务： [(600 - 150) / (2 * 2)] = 113;

第四个任务： [(450 - 113) / (2 * 2)] = 85;

第五个任务： [(337 - 252) / (2 * 2)] = 63 ,由于小于声明的80，所以这里为80；

第六个任务： 计算小于80，所以分配80

第七个任务： 计算小于80，所以根据声明，这里为80

第八个任务： 12，最后剩下12已小于80的迭代次数，所以为12

```
