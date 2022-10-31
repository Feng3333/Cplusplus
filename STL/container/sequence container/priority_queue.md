# priority_queue 优先队列

## 目录
  - [1. 基本介绍](#1-基本介绍)
  - [2. 基本定义](#2-基本定义)
  - [3. 基本类型举例](#3-基本类型举例)



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

在优先队列中当需要用自定义的数据类型时才需要传入这三个参数，使用基本数据类型时，只需要传入数据类型(priority_queue\<int> pq )，默认是大顶堆
  
### 常用方式
```cpp
//升序队列
priority_queue<int, vector<int>, greater<int>> pq;
  
//降序队列
priority_queue<int, vector<int>, less<int>> pq;
  
//greater和less是std实现的两个仿函数（就是使一个类的使用看上去像一个函数。其实现就是类中实现一个operator()，这个类就有了类似函数的行为，就是一个仿函数类了）
```
  
## 3. 基本类型举例
### 基本使用方法
```cpp
#include <iostream>
#include <queue>

using namespace std;

int main()
{
    priority_queue<int> a; //基础类型，默认为大顶堆
    // 等同于 priority_queue<int, vector<int>, less<int>> a;

    priority_queue<int, vector<int>, greater<int>> b; //小顶堆

    priority_queue<string> c;

    for (int i = 0; i < 5; i++) {
        a.push(i);
        b.push(i);
    }

    cout << "优先队列 a 中的元素为: " ;
    while (!a.empty()) {
        cout << a.top() << ' ';
        a.pop();
    }

    cout << endl << "优先队列 b 中的元素为: ";
    while (!b.empty()) {
       cout << b.top() << ' ';
       b.pop();
    }
    cout << endl;

    c.push("abc");
    c.push("abcd");
    c.push("cbd");
    cout << "c中元素为: ";
    while (!c.empty()) {
        cout << c.top() << " ";
        c.pop();
    }

    return 0;
}

```

### 使用pair之后的比较情况
先比较pair中第一个元素，若相等则比较第二个元素  
```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int main() 
{
    priority_queue<pair<int, int> > a;
    pair<int, int> b(1, 2);
    pair<int, int> c(1, 3);
    pair<int, int> d(2, 5);
    a.push(d);
    a.push(c);
    a.push(b);
    cout << "a中的元素为: " << endl;
    while (!a.empty()) 
    {
        cout << a.top().first << ' ' << a.top().second << '\n';
        a.pop();
    }
}
```
