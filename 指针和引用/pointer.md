# 指针

指针是一个变量，其存储的是值的地址，而不是值本身;  
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
