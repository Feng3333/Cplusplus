# function
注意：std::function是从C++11开始支持的特性，并且并不是STL的一部分，在这里记录只是因为本人纯纯的菜狗，

## 引言
### C语言中的函数指针
C语言中的函数指针可以实现C++中的多态；  
可以让函数指针指向参数类型相同、返回值类型也相同的函数；  
举个例子：  
```c++
#include <iostream>
typedef int (*func)();

int print1() {
    printf("hello,print1 \n");
    return 0;
}

int print2() {
    printf("hello,print2 \n");
    return 0;
}

int main(int argc, char *argv[]) {
    func fp = print1;
    fp();
    
    fp = print2;
    fp();
    
    return 0;
}
```
上述代码中定义了一个函数指针 func，它可以指向无输入参数，返回值为整型的函数。  
因此在main函数中，可以用fp(这里是func类型的指针)，分别指向了print1和print2并调用它们。
其运行结果如下：
```c++
hello,print1
hello,print2
```
## function的作用

在上述的示例代码中展示了c语言中函数指针的作用，在C++中的function也是具有这一功能的；  
另外，function比c语言中的函数指针功能更为强大些或者更适合在C++中使用(ps:我是没有看出来哪里强大了，或许是因为我理解的还不行吧)

结合示例说明function的使用方法:  
```c++
#include <iostream>
using namespace std;

void print3() {
    cout << "hello,print3" << endl;
}

void print4() {
    cout << "hello,print4" << endl; 
}

int main() {
    function<void()> func(&print1);
    func();
  
    func = &print2;
    func();
  
    return 0;
}
```
上述代码与C函数指针类似定义了两个全局函数print3和print4；  
在main函数中又定义了function对象func，然后将print3和print4分别赋值给func，这样就可以达到与C语言函数指针同样的功能了。
运行结果如下：
```c++
hello,print3
hello,print4
```
从运行结果可以看出function是C++中用来代替C函数指针的；  
实际上function还有一个比较有意思的用法：可以将一个重载了()的操作符的对象赋值给它，这样就可以想调用函数一样使用该对象了。  
具体示例：
```c++
...
struct A {
    void operator()() {
        std::cout << "This is A Object" << std::endl;
    }
};

int main(...) {
    ... 
      
    A a;
    func = a;
    func();
    ...
      
}
```
上述代码中，使用了struct定义了一个结构体，在该结构体中重载了()操作符,因此只要将A的类对象赋值给func，它就可以像函数一样使用了。
```c++
...
This is A Object  
```

## function的实现原理
https://zhuanlan.zhihu.com/p/161356621
