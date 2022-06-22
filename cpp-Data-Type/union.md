# union 共用体
## 基本介绍
union 是一种特殊的类，一个union中可以有多个数据成员;  

union可以存储不同的数据类型，但只能同时存储其中的一种类型；  

共用体每次只能存储一个值，因此必须由足够的空间来存储最大的成员，所以union的长度是由其最大成员的长度决定的；

举个例子：  
```c++
#include <iostream>

union val{
  char c_val;
  int i_val;
  double d_val;
};

int main() {
  union val num;
  num.i_val = 1;
  num.c_val = 'a';
  num.d_val = 0.12;
  
  std::cout << "num.i_val:" << num.i_val << std::endl;
  std::cout << "num.c_val:" << num.c_val << std::endl;
  std::cout << "num.d_val:" << num.d_val << std::endl;
  
  return 0;
}
```

## 节省空间？

共享内存：在union中所定义的任何一种数据，这些数据共享同一段内存，以达到节省空间的目的；
匿名共用体：
