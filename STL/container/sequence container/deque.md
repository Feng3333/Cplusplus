# deque : C++ STL中的双端队列

## 头文件
```cpp
#include <deque>
```

## 基本介绍
deque（double-ended queue）是一种双端队列，它是一种线性数据结构，可以在两端进行插入和删除操作。deque 的特点是可以在队列的两端进行快速的插入和删除操作，而且支持随机访问。  

在 C++ STL 中，deque 是一个模板类，定义在头文件 <deque> 中。deque 的定义如下：  
```cpp
template <class T, class Alloc = allocator<T>>
class deque;
```

## 基本接口函数
```cpp
#include <deque>

deque<int> dq;

int val = 3;
dp.push_back(val);   // 在队列的后端插入一个元素
dp.front_back(val);  // 在队列的前端插入一个元素
dp.pop_back();       // 删除队列的最后一个元素
dp.pop_front();      // 删除队列的第一个元素
dp.front();          // 返回队列的第一个元素
dp.back();           // 返回队列的最后一个元素
dp.size();           // 返回队列中的元素个数
dp.empty();          // 判断队列是否为空
dp.clear();          // 清空队列中的所有元素
```
