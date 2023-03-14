# C++ OpenMP

## 目录
 - [基本介绍](#基本介绍)
 - [具体实现](#具体实现)
 - [OpenMP常用函数](#openmp常用函数)
 - [for循环并行化的基本用法](#for循环并行化的基本用法)

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
上述代码需要编译后在执行:
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
