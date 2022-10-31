# priority_queue 优先队列

## 目录
- [1. 基本介绍](#1-基本介绍)
- [2. 基本定义](#2-基本定义)

## 1. 基本介绍
头文件:  
```c++
#include <queue>
```
优先队列具有队列的所有特性，包括基本操作，只是在这基础上增加了一个内部的排序机制，本质上是一个堆实现的  

```cpp
top() //访问队头元素;
empty() //判断队列是否为空
size() //返回队列中元素的个数
push() //插入元素，并且会自动排序
emplace() //原地构造一个元素并放入队列中
pop() //弹出队头元素
swap() //交换
```

## 2. 基本定义
```cpp
priority_queue<Type, Container, Functional>
```
Type: 描述优先队列中存放的数据类型  
Container: 容器类型，(Container中必须使用数组实现的容器，比如vector，queue等，但不能使用list，STL中优先队列默认使用vector)  
Functional: 优先队列中的比较方式，默认大顶堆  

在优先队列中当需要用自定义的数据类型时才需要传入这三个参数，  
使用基本数据类型时，只需要传入数据类型 (举个例子: priority_queue<int>  pq)，默认是大顶堆  
  
### 常用方式
```cpp
//升序队列
priority_queue<int, vector<int>, greater<int>> pq;
  
//降序队列
priority_queue<int, vector<int>, less<int>> pq;
  
//greater和less是std实现的两个仿函数（就是使一个类的使用看上去像一个函数。其实现就是类中实现一个operator()，这个类就有了类似函数的行为，就是一个仿函数类了）
```

  
  
