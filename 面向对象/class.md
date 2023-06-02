# class

## 目录
 - [1.基本介绍](#1-基本介绍)
 - [2.对象的初始化和清理](#2-对象的初始化和清理)

## 1.基本介绍
C++中使用class关键字来创建一个新的对象:
- class中有三种类型的封装：public、 private、protected；
- 区别于struct结构体，class里面的成员默认是private的;

## 2.对象的初始化和清理
C++利用构造函数和析构函数解决对象的初始化和清理工作，这两个函数会被编译器自动调用；  
- 构造函数：主要作用在于创建对象时为对象的成员属性进行赋值，构造函数由编译器自动调用；  
- 析构函数：主要作用在于对象销毁前系统自动调用，执行一些清理工作；  

### 构造函数
#### 构造函数语法 : 类名(){}
```c++
#include <iostream>
#include <string>

#ifndef TEST_H
#define TEST_H

class MyClass{
public:
    MyClass(){}; //基础的构造函数

private:

};

#endif // TEST_H
```
1. 构造函数，没有返回值也不写void  
2. 函数名称与类名相同  
3. 构造函数可以有参数，因此可以发生重载
4. 程序在调用对象时会自动调用构造函数，无需手动调用，而且只会调用一次  

#### 析构函数语法 ：~类名(){};
1. 析构函数，没有返回值也不写void   
2. 析构函数名称与类名相同，但在名称前要加上符号~  
3. 析构函数不可以有参数，因此不可以发生重载
4. 程序在对象销毁前会自动调用构造函数，无需手动调用，而且只会调用一次  

基础代码用例： 
```c++
#include <iostream>
#include <string>

#ifndef TEST_H
#define TEST_H

class MyClass{
public:
    MyClass() {
        std::cout << "MyClass 构造函数" << std::endl;
    };

    ~MyClass() {
        std::cout << "MyClass 析构函数" << std::endl;
    };

private:

};

#endif // TEST_H
```
```c++
#include "test.h"

int main()
{
    MyClass myClass;
    return 0;
}
```

#### 构造函数的分类及调用
- 注意：构造函数是不可以成为虚函数的， 虚函数是依赖虚函数表存在的，而只用创建了对象之后虚函数表才会存在，所以，构造函数不能成为虚函数  
- 两种分类方式：  
  按参数类型分：有参构造和无参构造；  
  按类型分：普通构造和拷贝构造；  
- 三种调用方式：括号法，显示法，隐式转换法  

代码示例：
```c++

```
