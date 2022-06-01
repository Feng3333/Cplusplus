# unique_ptr的基本介绍
unique_ptr是定义在头文件 \<memory\> 中的智能指针。  
```c++
#include <memory>
```

## unique_ptr的基本规则
①：持有对对象的独有权---->即两个unique_ptr不能指向同一个对象，unique_ptr不共享他所管理的对象;  
②：无法复制到其他unique_ptr，无法通过值传递到函数，也无法用于需要副本的任何标准模板库算法；  
③：只能移动 unique_ptr，即对资源管理权限可以实现转移。----->这意味着，内存资源所有权可以转移到另一个 unique_ptr，并且原始 unique_ptr 不再拥有此资源;    
④：unique_ptr 所持有的对象销毁时会释放其持有的堆内存  

下图就演示了两个unique_ptr实例之间的所有权转换  
![iamge](https://github.com/Feng3333/Cplusplus/blob/26d819b8921fbf5b60815d249c82095af03ab822/images-folder/unique_ptr1.png)

## unique_ptr的基本操作
unique_ptr的基本操作有：
```c++
#include <memory>
#include <iostream>
#include <string>

int main () {
  //创建空智能指针
  std::unique_ptr<int> u_i1; 
  //绑定动态对象
  u_i1.reset(new int(3));
  
  //创建时指定动态对象
  std::unique_ptr<int> u_i2(new int(6));
  //std::cout << &u_i2 <<std::endl << *u_i2 << std::endl; 
  
  //创建空 unique_ptr，执行类型为 T 的对象，用类型为 D 的对象 d 来替代默认的删除器 delete
  //unique_ptr<T,D> u_d1(d);	

  //所有权的变化：
  int *p1 = u_i2.release(); //释放所有权
  //std::cout << *p1 << std::endl << (u_i2 == nullptr) << std::endl;
  
  std::unique_ptr<std::string> u_s1(new std::string("abcdefg"));
  //std::cout << *u_s1 << std::endl << &u_s1 << std::endl;
  
  std::unique_ptr<std::string> u_s2 = std::move(u_s1); //所有权转移，u_s1变成空指针
  //std::cout << *u_s2 << std::endl << &u_s2 << std::endl;
  
  u_s2.reset(u_s1.release()); //所有权转移
  //std::cout << *u_s2 << std::endl << &u_s2 << std::endl;
  u_s2 = nullptr; //显示销毁所指对象，同时u_s2变成空指针,与reset()等价

  return 0;
}
```


需要注意的点：  

unique_ptr 与原始指针一样有效，并可用于 STL 容器。将 unique_ptr 实例添加到 STL 容器运行效率很高，因为通过 unique_ptr 的移动构造函数，不再需要进行复制操作。unique_ptr 指针与其所指对象的关系：在智能指针生命周期内，可以改变智能指针所指对象，如创建智能指针时通过构造函数指定、通过 reset 方法重新指定、通过 release 方法释放所有权、通过移动语义转移所有权，unique_ptr 还可能没有对象，这种情况被称为 empty
