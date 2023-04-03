# C++入栈顺序
c或c++中的都是从右往左入栈的，在调用函数时，若参数需要运算则先运算，然后入栈，在调用函数

## 目录
 - [1. 入栈顺序](#1-入栈顺序)

## 1. 入栈顺序
- 栈区是从高地址向低地址扩展，是一块连续的内存区域，遵循先进后出，后进先出(FILO)原则;
- 程序中栈顶在低位，栈底在高位  
程序的入栈顺序演示：

```c++
#include <iostream>

void push(int a, int b, int c) 
{
    std::cout << "a = " << a << ", ptr = " << &a << std::endl;
    std::cout << "b = " << b << ", ptr = " << &b << std::endl;
    std::cout << "c = " << c << ", ptr = " << &c << std::endl;
}

int main() {
    int a = 0, b = 1, c = 2;
    push(a, b, c);
    return 0;
}
```

输出结果 :
```
a = 0, ptr = 0x7fffcba586bc
b = 1, ptr = 0x7fffcba586b8
c = 2, ptr = 0x7fffcba586b4
```
解释说明：
