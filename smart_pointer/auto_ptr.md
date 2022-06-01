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
