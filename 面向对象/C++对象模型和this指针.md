# C++对象模型和this指针

# 目录
 - [1. 成员变量和成员函数分开存储](#1-成员变量和成员函数分开存储)
 - [2. this指针](#2-this指针)
 - [3. 空指针访问成员函数](#3-空指针访问成员函数)
 - [4. const修饰的成员函数](#4-const修饰的成员函数)
 
# 1. 成员变量和成员函数分开存储
在C++中，类内的成员变量和成员函数分开存储，只有非静态成员变量才属于类的对象上
```c++
class Student{};

class Person{
public:
    Person() {
        mA = 0;
    }

    // 函数不占用对象空间，所有函数共享一个函数实例
    void func() {
        std::cout << "mA : " << mA << std::endl;
    }

    // 静态成员函数也不占用对象空间
    static void s_func() {
        std::cout << "mB : " << mB << std::endl;
    }

public:
    int mA;  // 非静态成员变量会占用对象空间 
    static int mB;  // 静态成员变量不占用对象空间
};

int main()
{
    Student s;
    std::cout << "sizeof(s) : " << sizeof(s) << std::endl;
    std::cout << "sizeof(Person) : " << sizeof(Person) << std::endl;
    return 0;
}
```

# 2. this指针
this指针指向被调用的成员函数所属的对象；  
this指针时隐含每一个非静态成员函数内的一种指针；  
this指针不需要定义，直接使用即可；  

## this指针的用途
- 当形参和成员变量同名时，可以用this指针来区分
- 在类的非静态成员函数中返回对象本身，可使用return \*this
```cpp
#include <string>
#include <iostream>

class Person{
public:
    Person(int age) {
        // 当形参与成员变量同名时，可以用this指针来区分
        this->age = age;
    }

    // Person PersonAddPerson是值传递，会调用拷贝函数
    // Person& PersonAddPerson是地址传递 
    Person& PersonAddPerson(Person p) {
        this->age += p.age;
        return *this;
    }

public:
    int age;
};

int main()
{
    Person p1(10);
    std::cout << "p1.age : " << p1.age << std::endl;

    Person p2(16);
    p2.PersonAddPerson(p1).PersonAddPerson(p1).PersonAddPerson(p1);
    std::cout << "p2.age : " << p2.age << std::endl;
    return 0;
}
```

# 3. 空指针访问成员函数
C++中空指针也是可以调用成员，但是需要注意有没有用到this指针；  
如果用到了this指针，需要加以判断保证代码的健壮性。  

代码示例：
```cpp
#include <string>
#include <iostream>

class Person{
public:
    void ShowClassName() {
        std::cout << "Person类 " << std::endl; 
    }

    void ShowPerson() {
        if (this == nullptr) {
            std::cout << "this == nullptr" << std::endl;
            return;
        }
        std::cout << mAge << std::endl;
    }

public:
    int mAge;
};

int main()
{
    Person *p = nullptr;
    p->ShowClassName();
    p->ShowPerson();
    return 0;
}
```

# 4. const修饰的成员函数
## 常函数：
- 成员函数后加上const称该函数为常函数；  
- 特点：  
  - 常函数内不可以修改成员属性；  
  - 成员属性声明时加上关键字mutable后，在常函数中依然可以修改

## 常对象：
- 声明对象前加上const称该对象为常对象；
- 特点：
  - 常对象只能调用常函数；
  - 常对象不能修改成员变量的值，但可以访问；
  - 常对象可以修改mutable修饰的成员变量；

## 代码示例： 
```cpp
#include <string>
#include <iostream>

class Person{
public:
    Person() {
        m_a = 0;
        m_b = 1;
    }
    /* this指针的本质是一个指针常量，指针的指向不可修改；
       如果想要this指针指向的值也不可修改，需要声明为常函数 */
    void ShowPerson() const {
        // this->m_a = 15; // 报错常函数中指针指向的值(没有添加mutable时)不可修改
        this->m_b = 100;
    }

public:
    int m_a;
    mutable int m_b; // 可修改
};

int main()
{
    const Person p; // 常量对象
    std::cout << "p.m_a : " << p.m_a << std::endl;
    p.m_b = 2;
    std::cout << "p.m_b : " << p.m_b << std::endl;

    p.ShowPerson();
    std::cout << "After p.ShowPerson(), p.m_b : " << p.m_b << std::endl;
    return 0;
}

```
