# C++ 中的static
C++中的static主要有两种用法: 面向过程程序设计中的static 和 面向对象程序设计中的static. 前者应用于普通变量和函数,不涉及类; 后者主要说明static在类中的作用. 
## 目录
- [1. 面向过程程序设计中的static](#1-面向过程程序设计中的static)
- [2. 面向对象程序设计中的static](#2-面向对象程序设计中的static)

## 1. 面向过程程序设计中的static
### 1.1 静态全局变量
在全局变量前, 加上关键字static, 该变量便被定义为静态全局变量  
简单举例:  
```c++
#include <iostream>

static int num; //静态全局变量

void fn()
{
    num++;
    std::cout << "num : " << num << std::endl;
}

int main() {
    num = 20;
    std::cout << "num : " << num << std::endl;
    fn();
    return 0;
}
```
静态全局变量具有以下特点: 
- 该变量在全局数据区分配内存;
- 未经初始化的静态全局变量会被程序自动初始化为0 (自动变量的值是随机的, 除非它被显示初始化);
- 静态全局变量在声明它的整个文件都是可见的, 而在文件之外是不可见的;

## 2. 面向对象程序设计中的static
