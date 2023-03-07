# C++ OpenMP

## 目录


## 基本介绍
头文件
```
#include <omp.h>
```
OpenMP:多线程文件操作  
OpenMP是由OpenMP Architecture Review Board牵头提出的，并已被广泛接受，用于共享内存并行系统的多处理器程序设计的一套指导性编译处理方案(Compiler Directive)  
OpenMP支持的编程语言包括C、C++和Fortran；而支持OpenMp的编译器包括Sun Compiler，GNU Compiler和Intel Compiler等。  

OpenMp提供了对并行算法的高层的抽象描述，程序员通过在源代码中加入专用的pragma来指明自己的意图，由此编译器可以自动将程序进行并行化，并在必要之处加入同步互斥以及通信。当选择忽略这些pragma，或者编译器不支持OpenMp时，程序又可退化为通常的程序(一般为串行)，代码仍然可以正常运作，只是不能利用多线程来加速程序执行。

## 基本实现
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
