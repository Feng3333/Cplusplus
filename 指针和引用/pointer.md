# 指针
## 指针的基本介绍
指针是一个变量，其存储的是值的地址，而不是值本身;可以认为指针其实就相当于地址.  
使用 & 地址运算符可以获取变量的地址;  
举个例子:  
```c++
#include <iostream>

using namespace std;

int main(){
  int num1 = 6;
  double num2 = 4.5;
  cout << "num1: " << num1 << " address: " << &num1 << endl;
  cout << "num1: " << num1 << " address: " << &num1 << endl;
  
  return 0;
}
```

## 指针的声明和初始化

声明指针必须先指定指针指向的数据的类型！！
```cpp
int* p1;
char* p2;
int* p3, p4 //声明了一个int型指针p3，int型数据p4------->对于每个指针变量名，都需要使用一个*
```  



