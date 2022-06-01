 # shared_ptr（引用计数型智能指针）
 
 ## shared_ptr简介
 shared_ptr 是一个标准的共享所有权的智能指针，允许多个指针指向同一个对象;  
 shared_ptr 是利用引用计数的方式实现了对所管理的对象的所有权的分享，即允许多个 shared_ptr 共同管理同一个对象  
 
 每次拷贝一个shared_ptr都会使这个引用计数递增（当局部的shared_ptr离开作用域时，计数器就会递减），当引用计数为0（指向对象的最后一个shared_ptr被销毁）时，shared_ptr会销毁这个对象（通过shared_ptr的析构函数）可以使用use_count()方法来获取当前持有资源的引用计数。
 
## shared_ptr的初始化：
 ```c++
#include <iostream>
#include <memory>
 
int main() {
  std::shared_ptr<int> sp1(new int(333)); //只能使用直接初始化
  
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
  
  sp2.reset();
  std::cout << "sp2.get() == nullptr ? " << (sp2.get() == nullptr) << std::endl;
  std::cout << "当前引用计数：" << sp1.use_count() << std::endl;
  
  auto sp3 = std::make_shared<int> (666);
  std::cout << "sp3:" << *sp3 << std::endl;
  sp3.swap(sp1);
  std::cout << "sp1:" << *sp1 << "\t" << "sp3:" << *sp3 << std::endl;
  
  sp1=nullptr;
  std::cout << "sp1 is empty?" <<  "\t" << sp1.unique() << std::endl;
  sp1 = std::make_shared<int>(999);
  std::cout << "sp1:" << *sp1 <<std::endl;
  
  return 0;
}
 ```
