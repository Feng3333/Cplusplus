# class

## 目录
 - [1.基本介绍](#1-基本介绍)
 - [2.对象的初始化和清理](#2-对象的初始化和清理)
   - [构造函数和析构函数](#构造函数和析构函数)
   - [构造函数的分类及调用](#构造函数的分类及调用)
   - [拷贝构造函数调用时机](#拷贝构造函数调用时机)
   - [构造函数调用规则](#构造函数调用规则)
   - [深拷贝和浅拷贝](#深拷贝和浅拷贝)

## 1. 基本介绍
C++中使用class关键字来创建一个新的对象:
- class中有三种类型的封装：public、 private、protected；
- 区别于struct结构体，class里面的成员默认是private的;

## 2. 对象的初始化和清理
C++利用构造函数和析构函数解决对象的初始化和清理工作，这两个函数会被编译器自动调用；  
- 构造函数：主要作用在于创建对象时为对象的成员属性进行赋值，构造函数由编译器自动调用；  
- 析构函数：主要作用在于对象销毁前系统自动调用，执行一些清理工作；  

### 构造函数和析构函数
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
2. 函数名称与类名相同  构造函数
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

### 构造函数的分类及调用
- 注意：构造函数是不可以成为虚函数的， 虚函数是依赖虚函数表存在的，而只用创建了对象之后虚函数表才会存在，所以，构造函数不能成为虚函数  
- 两种分类方式：  
  按参数类型分：有参构造和无参构造；  
  按类型分：普通构造和拷贝构造；  
- 三种调用方式：括号法，显示法，隐式转换法  

代码示例：
```c++
#include <iostream>
#include <string>

#ifndef TEST_H
#define TEST_H

class Person {
public:
    Person() {
        std::cout << " 无参数构造函数 " << std::endl;
    }

    Person(int num) {
        age = num;
        std::cout << " 有参数构造函数 " << std::endl;
    }

    Person(const Person& p) {
        age = p.age;
        std::cout << " 拷贝构造函数 " << std::endl;
    }
    
    ~Person() {
        std::cout << " 析构函数 " << std::endl;
    }
    
public:
    int32_t age;
};

#endif // TEST_H
```
```c++
#include "test.h"

void test01()
{
    Person p; //调用无参数的构造函数
    /* Person p() 调用无参数构造函数不能加括号，如果加了编译器就会认为这是一个函数声明 */
}

// 调用有参数的构造函数
void test02()
{   
    // 括号法，
    Person p1(10);

    // 显式法
    Person p2 = Person(15);
    Person p3 = Person(p2);

    // 隐式转换法
    Person p4 = 20; // Person p4 = Person(20)
    Person p5 = p4; // Person p5 = Person(p4)

    // 不能利用拷贝构造函数 初始化匿名对象，否则编译器会认为是对象声明
    // Person p6(p5);
}

int main()
{
    std::cout << "test01 : " << std::endl;
    test01();
    std::cout << "test02 : " << std::endl;
    test02();
    return 0;
}
```

### 拷贝构造函数调用时机
C++中拷贝构造函数调用的时机通常有三种情况：  
- 使用一个已经创建完毕的对象来初始化一个新的对象
- 值传递的方式给函数参数传值
- 以值方式返回局部对象

```cpp
#include <iostream>
#include <string>

#ifndef TEST_H
#define TEST_H

class Person {
public:
    Person() {
        std::cout << " 无参数构造函数 " << std::endl;
        mAge = 0;
    }

    Person(int num) {
        mAge = num;
        std::cout << " 有参数构造函数 " << std::endl;
    }

    Person(const Person& p) {
        mAge = p.mAge;
        std::cout << " 拷贝构造函数 " << std::endl;
    }
    
    ~Person() {
        std::cout << " 析构函数 " << std::endl;
    }

public:
    int32_t mAge;
};

#endif // TEST_H
```
```cpp
#include "test.h"

// 使用一个已经创建完毕的对象来初始化一个新对象
void Test01()
{
    Person man(100);
    Person newMan(man); // 调用拷贝构造函数
    Person newMan2 = man; // 拷贝构造
}

// 值传递的方式给函数传参数
void DoWork(Person p1) {};
void Test02()
{   
    Person p;
    DoWork(p);
}

// 以值的方式返回局部变量
Person DoWork2()
{
    Person p1;
    std::cout << (int *)&p1 << std::endl;
    return p1;
}

void Test03()
{
    Person p = DoWork2();
    std::cout << (int *)&p << std::endl;
}

int main()
{
    std::cout << "Test01 : " << std::endl;
    Test01();
    std::cout << "Test02 : " << std::endl;
    Test02();
    std::cout << "Test03 : " << std::endl;
    Test03();
    return 0;
}
```

### 构造函数调用规则
默认情况下，C++编译器至少会给一个类添加三个函数：  
- 默认构造函数（无参数，函数体为空） 
- 默认析构函数（无参数，函数体为空）
- 默认拷贝构造函数，对属性值进行值拷贝

构造函数的调用规则如下：  
- 如果用户定义有参构造函数，C++不在提供默认无参构造，但是会提供默认拷贝构造 
- 如果用户定义拷贝构造函数，C++不会在提供其他构造函数  

代码示例：
```cpp
#include <iostream>
#include <string>

#ifndef TEST_H
#define TEST_H

class Person {
public:
    Person() {
        std::cout << " 无参数构造函数 " << std::endl;
        mAge = 0;
    }

    Person(int num) {
        mAge = num;
        std::cout << " 有参数构造函数 " << std::endl;
    }

    Person(const Person& p) {
        mAge = p.mAge;
        std::cout << " 拷贝构造函数 " << std::endl;
    }
    
    ~Person() {
        std::cout << " 析构函数 " << std::endl;
    }

public:
    int32_t mAge;
};

#endif // TEST_H
```
```cpp
#include "test.h"

void Test_1()
{
    Person p1(18);
    Person p2(p1);  // 如果不写拷贝构造，编译器会自动添加拷贝构造，并且做浅拷贝构造操作
    std::cout << "p2's age is : " << p2.mAge << std::endl;
}

void Test_2()
{
    /* 如果用户提供有参构造，编译器不会提供默认构造，会提供拷贝构造 */
    Person p1;   // 此时如果用户自己没有提供默认构造，会出错
    Person p2(10);
    Person p3(p2); // 此时如果用户没有提供拷贝构造，编译器会提供

    /* 如果用户提供了拷贝构造，编译器不会提供其他构造函数 */
    Person p4;  // 此时如果用户自己没有提供默认构造，会出错
    Person p5(15); // 此时如果用户自己没有提供有参数的构造函数，会出错
    Person p6(p5); // 用户自己提供了拷贝构造
}

int main()
{
    std::cout << "Test_1 : " << std::endl;
    Test_1();
    std::cout << "Test_2 : " << std::endl;
    Test_2();
    return 0;
}
```
### 深拷贝和浅拷贝
- 浅拷贝：  
  简单的赋值拷贝操作
- 深拷贝：  
  在堆区重新申请空间，进行拷贝操作
  
代码示例：
```cpp
#include <iostream>
#include <string>

#ifndef TEST_H
#define TEST_H

class Person {
public:
    Person() {
        std::cout << " 无参数构造函数 " << std::endl;
    }

    Person(int num, int height) {
        std::cout << " 有参数构造函数 " << std::endl;
        mAge = num;
        mHeight = new int(height);
    }

    Person(const Person& p) {
        std::cout << " 拷贝构造函数 " << std::endl;
        mAge = p.mAge;
        // 如果不利用深拷贝在堆区创建新内存，会导致浅拷贝带来的重复释放堆区问题
        mHeight = new int(*p.mHeight);
    }
    
    ~Person() {
        std::cout << " 析构函数 " << std::endl;
        if (mHeight != nullptr) {
            delete mHeight;
        }
    }

public:
    int32_t mAge;
    int32_t* mHeight;
};

#endif // TEST_H
```
```cpp
#include "test.h"

void Test_1()
{
    Person p1(18, 175);
    Person p2(p1); 
    std::cout << "p1's age is : " << p1.mAge << " height : " << *p1.mHeight << std::endl;
    std::cout << "p2's age is : " << p2.mAge << " height : " << *p2.mHeight << std::endl;
}

int main()
{
    std::cout << "Test_1 : " << std::endl;
    Test_1();
    return 0;
}
```
