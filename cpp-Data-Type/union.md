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
匿名共用体： 直观感受，由于地址union的地址相同，所以可以不需要中间的标识符；
```c++
#include <iostream>
#include <string>

struct player{
  std::string name;
  union {
    int i_age;
    double d_age;
  };
};

int main() {
  player p;
  p.name = "Messi";
  p.i_age = 35;
  std::cout << "name :" << p.name << " i_age: " << p.i_age << std::endl;
  p.d_age = 36;
  std::cout << "name :" << p.name << " d_age: " << p.d_age << std::endl;
  
  return 0;
}

```

节省代码量和api
```c++
union DATA {
  char c_data;
  int i_data;
  double d_data;
};

void printData(DATA data,char type) {
  switch(type){
    case 0: 
      printf("%c\n", data.c_data); 
      break;
    case 1: 
      printf("%c\n", data.i_data); 
      break;
    case 2: 
      printf("%c\n", data.d_data); 
      break;
    default:
      break;
  }
}
```
