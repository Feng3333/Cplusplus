 # shared_ptr（引用计数型智能指针）
 
 ## shared_ptr简介
 shared_ptr 是一个标准的共享所有权的智能指针，允许多个指针指向同一个对象;  
 shared_ptr 是利用引用计数的方式实现了对所管理的对象的所有权的分享，即允许多个 shared_ptr 共同管理同一个对象  
 
 每次拷贝一个shared_ptr都会使这个引用计数递增（当局部的shared_ptr离开作用域时，计数器就会递减），当引用计数为0（指向对象的最后一个shared_ptr被销毁）时，shared_ptr会销毁这个对象（通过shared_ptr的析构函数）可以使用use_count()方法来获取当前持有资源的引用计数。
 
 #### 每个 shared_ptr 对象在内部指向两个内存位置：
 1、指向对象的指针；  
 2、用于控制引用计数的数据的指针；  
 
 #### 共享所有权如何在参考计数的帮助下工作：
 1、当新的 shared_ptr 对象与指针关联时，则在其构造函数中，将于此指针关联的引用计数+1；
 2、当任何 shared_ptr 对象超出作用域时，则在其析构函数中，它将关联指针的引用计数 -1 ，如果引用计数变为0，则表示没有其他 shared_ptr 对象与次内存相关联，这种情况下，它将使用 delete函数删除该内存
 
## shared_ptr的初始化：
 ```cpp
#include <iostream>
#include <memory>
 
int main() {
  //只能使用直接初始化
  std::shared_ptr<int> sp1(new int(333)); //这行代码在堆上创建了两块内存：1：存储int。2：用于引用计数的内存，管理附加此内存的 shared_ptr 对象的计数，最初计数将为1。
  
  
  std::shared_ptr<int> sp2;
  sp2.reset(new int(333));
  
  std::shared_ptr<int> sp3;
  sp3 = std::make_shared<int>(333);
  
  return 0;
}
 ```
 ## shared_ptr的基本操作
 ```c++
#include <iostream>
#include <memory>
 
int main() {
  auto sp1 = std::make_shared<int>(333);
  std::cout << "初始化sp1:" << *sp1 << std::endl;
  std::cout << "当前引用计数：" << sp1.use_count() << std::endl;
  
  auto sp2 = sp1;
  std::cout << "sp2:" << *sp2 << std::endl;
  std::cout << "当前引用计数：" << sp1.use_count() << std::endl;
  
  sp2.reset(); //重置当前存储的指针
  std::cout << "sp2.get() == nullptr ? " << (sp2.get() == nullptr) << std::endl;
  std::cout << "当前引用计数：" << sp1.use_count() << std::endl;
  
  auto sp3 = std::make_shared<int> (666);
  std::cout << "sp3:" << *sp3 << std::endl;
  //交换两个指针指向的对象，注意，这里数据类型要相同才可以交换
  sp3.swap(sp1);
  std::cout << "sp1:" << *sp1 << "\t" << "sp3:" << *sp3 << std::endl;
  
  sp1=nullptr;
  //unique() 检查当前shared_ptr是不是该对象的唯一持有者 (C++20 被抛弃）
  std::cout << "sp1 is empty?" <<  "\t" << sp1.unique() << std::endl;
  sp1 = std::make_shared<int>(999);
  std::cout << "sp1:" << *sp1 <<std::endl;
  
  return 0;
}
 ```

## 通过模拟实现 shared_ptr 来理解 shared_ptr的使用和原理

#### 基础对象类
定义的基础对象类 ：Point类，为了后续验证 自定义的智能指针是否有效:
```cpp
class Point{
private:
  int x,y;
  
public:
  Point(int xVal = 0, int yVal = 0 ) : x(xVal), y(yVal) {}
  
  int getX() const {
    return x;
  }
  
   int getY() const {
    return y;
  }
  
  void setX(int xVal) {
    x = xVal
  }
  
  void setY(int yVal) {
    y = yVal
  }
  
};
```

#### 辅助类
辅助类用以封装使用计数与基础对象指针；  
这个类的所有成员皆为私有类型，只为智能指针使用，还需要把智能指针类声明为辅助类的友元。这个辅助类含有两个数据成员：计数 count 与基础对象指针  
```cpp
class RefPtr {
  private:
    friend class SmartPtr;
    int count;
    Point *p;
    
    RefPtr(Point* ptr) : p(ptr),count(1){}
    ~RefPtr() {
      delete p;
    }
    
};
```

#### 为基础对象类实现智能指针类
使用引用计数实现：将一个计数器与类指向的对象相关联，引用计数跟踪共有多少个对象  
1、当创建智能指针类的新对象时，初始化指针，并将引用计数 count + 1;  
2、当智能指针类对象作用为另一个对象的副本时：拷贝构造函数复制副本的指向辅助类对象的指针，并增加辅助类对象对基础类对象的引用计数；  
3、使用赋值操作符对一个智能指针类对象进行赋值时：先要使左操作数的引用计数 -1 （此时指针指向了别的地方），如果此时引用计数为0了，则需要释放指针指向的内存，然后再增加右操作数所指对象的引用计数（此时左操作数指向的对象即为有操作数指向的对象）；  
4、析构：调用析构函数时，要先使引用计数 -1，如果引用计数为0了则需要delete对象；  
```cpp
class SmartPtr {
public:
  //构造函数
  SmartPtr() {
    rp = nullptr;
  }
  SmartPtr(Point* ptr): rp(new RefPtr(ptr)) {}
  //拷贝构造
  SmartPtr(const SmartPtr &sp): rp(sp.rp) {
    ++rp->count;
    std::cout << "copy constructor" << std::endl;
  }
  
  //重载赋值运算符
  SmartPtr& operator=(const SmartPtr& rhs) {
    ++rhs.rp->count;
    if (rp != nullptr && --rp->count == 0) {
      delete rp;
    }
    rp = rhs.rp;
    std::cout << " = operator" << std::endl;
    return *this;
  }
  
  //重载->运算符
  Point* operator->() {
    return rp->p;
  }
  
  // 重载*操作符
  Point& operator*() {
    return *(rp->p);
  }
  
  //析构函数
  ~SmartPtr() {
    if (--rp->count == 0) {
      std::cout << "引用计数变为0,将释放该指针指向的内存" << std::endl;
      delete rp;
    }
    else {
      std::cout << "还有" << rp->count << "个指针指向该对象" << std::endl;
    }
  }
  

private:
  RefPtr* rp;
};
```

#### 自定义智能指针的测试
```cpp
int main() {
  //定义一个基础对象指针类
  Point *pa = new Point(10,20);
  
  //使用{}来控制智能指针的生命周期
  {
    SmartPtr sptr1(pa);
    std::cout << "sptr1 : " << std::endl <<" x: " << sptr1->getX() << ", y: " << sptr1->getY() << std::endl;
    {
      SmartPtr sptr2(sptr1);
      std::cout << "sptr2 : " << std::endl <<" x: " << sptr2->getX() << ", y: " << sptr2->getY() << std::endl;
      {
        SmartPtr sptr3;
        sptr3 = sptr1;
        std::cout << "sptr3 : " << std::endl <<" x: " << sptr3->getX() << ", y: " << sptr3->getY() << std::endl;
      }
    }
  }
  
  std::cout << pa->getX() << std::endl;
  
  return 0;
}
```

测试结果如下：  
![image](https://github.com/Feng3333/Cplusplus/blob/ec0124a6f6b569b96e6d19e82bcfe63ca6f67002/images-folder/SmartPtr1.png)
