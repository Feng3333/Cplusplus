# auto_ptr
头文件：  
```c++
#include <memory>
```
## auto_ptr的基本用法
```c++
#include <memory>
#include <iostream>

int main()
{
  //初始化方式1
  std::auto_ptr<int> a1(new int(8));
  //初始化方式2
  std::auto_ptr<int> a2;
  std::cout << *a1 << endl;
  ap2.reset(new int(8));
  return 0;
}
```

## auto_ptr与unique_ptr的主要区别

当复制一个 auto_ptr 对象时（拷贝复制或 operator= 复制），原对象所持有的堆内存对象也会转移给复制出来的对象；
而 unnique_ptr 是无法复制到其他unique_ptr，无法通过值传递到函数,只能移动 unique_ptr;

举个栗子：
```c++
#include <iostream>
#include <memory>

int main(){
  //测试拷贝构造
  std::auto_ptr<int> a1(new int(3));
  std::auto_ptr<int> a2(a1);
  
  std::cout << *a2 << "\t" << &a2 << std::endl;
  if (a1.get() == NULL) {
    std::cout << "a1 is empty!!" << std::endl;
  }
  else {
    std::cout << "a1 is not empty" << std::endl;
  }
  
  //测试赋值构造
  std::auto_ptr<int> a3(new int(6));
  std::auto_ptr<int> a4;
  a4 = a3;
  std::cout << *a4 << "\t" << &a4 << std::endl;
  if (a3.get() == NULL) {
    std::cout << "a3 is empty!!" << std::endl;
  }
  else {
    std::cout << "a3 is not empty" << std::endl;
  }
  
  return 0;
}
```
上述代码中 a1 和 a3 由于经历的拷贝构造和赋值构造创建了新的对象，所有 a1 和 a3 指向的对象变为空了 ，这时候在对 a1 和 a3 进行访问就会出现内存报错的问题；

举个容易出错的栗子：
```c++
#include <iostream>
#include <vector>
#include <memory>

int main() {
    std::auto_ptr<int>  nums[5] = {
    std::auto_ptr<int> (new int(1)),
    std::auto_ptr<int> (new int(2)),
    std::auto_ptr<int> (new int(3)),
    std::auto_ptr<int> (new int(4)),
    std::auto_ptr<int> (new int(5)),
  };
  
  for(int i=0; i<5; i++) {
    std::cout << *nums[i] << std::endl;
  }
  
  std::auto_ptr<int> changenum;
  
  changenum = nums[3];
  std::cout << "changenum = " <<*changenum << std::endl;
  for(int i=0; i<5; i++) {
    std::cout << *nums[i] << std::endl;
  }
  
  return 0;
}
```
## 摒弃auto_ptr的原因？？

一句话概括：避免因潜在的内存问题导致程序崩溃。  
上述栗子中：  
如果使用 unique_ptr ，那么直接使用赋值传递时便会出错（编译器会提醒你 unique_ptr 不支持 = 的操作）； 而auto_ptr 有拷贝语义，拷贝后原对象变得无效，再次访问原对象时会导致程序崩溃

