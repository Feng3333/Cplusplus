# weak_ptr

## weak_ptr简介
weak_ptr 被设计为与 shared_ptr 共同工作，可以从一个 shared_ptr 或者另一个 weak_ptr 对象构造而来。  
weak_ptr 是为了配合 shared_ptr 而引入的一种智能指针，它更像是 shared_ptr 的一个助手而不是智能指针，因为它不具有普通指针的行为，没有重载 operator* 和 operator->;  
weak_ptr 的最大作用在于协助 shared_ptr 工作，可获得资源的观测权，像旁观者那样观测资源的使用情况;  
观察者意味着 weak_ptr 只对 shared_ptr 进行引用，而不改变其引用计数，当被观察的 shared_ptr 失效后，相应的 weak_ptr 也相应失效  


## weak_ptr基本用法
```cpp
weak_ptr<T> w; //创建空的weak_ptr指针，可以指向类型为T的对象
weak_ptr<T> w(sp); //与shared_ptr指向相同的对象，shared_ptr的引用计数不会发生改变，T必须是能装换为 sp 指向的类型
w=p;  // p可以是 shared_ptr 或者 weak_ptr,赋值后 w 与 p 共享对象

w.reset(); //将w置空
w.use_count(); //返回与 w 共享对象的 shared_ptr 的数量
w.expired(); //若w.use_count()为0，返回true，否则返回false
w.lock(); //如果 expired()为true，则返回一个空的shared_ptr,否则返回非空的 shared_ptr;
```

## 简单的使用事例
```cpp
#include<iostream>
#include<memory>

int main() {
  auto sp1 = std::make_shared<int> (3);
  std::cout << "当前sp1指向的值为：" << *sp1 << std::endl;
  std::cout << "sp1.use_count: " << sp1.use_count() << std::endl;
  std::weak_ptr<int> wp1(sp1);
  std::cout << "wp1.use_count: " << wp1.use_count() << std::endl;
  
  if (!wp1.expired()) {
    std::shared_ptr<int> sp2 = wp1.lock();
    *sp2 = 6;
    std::cout << "wp1.use_count: " << wp1.use_count() << std::endl;
  }
  
  std::cout << "当前sp1指向的值为：" << *sp1 << std::endl;
  std::cout << "sp1.use_count: " << sp1.use_count() << std::endl;
  
  return 0;
}

```

## weak_ptr
其实 weak_ptr 可用于打破循环引用；  
weak_ptr 对象引用资源时不会增加引用计数，但是它能够通过 lock() 方法来判断它所管理的资源是否被释放  
