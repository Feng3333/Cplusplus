# set
头文件
```c++
#include <set>
```
## 目录
 - [1. set的基本介绍](#1-set的基本介绍)
 - [2. set的基本操作](#2-set的基本操作)
 - [3. 遍历set的方法](#3-遍历set的方法)

## 1. set的基本介绍
- 底层实现为：红黑二叉树;  
- set容器内的元素会自动排序，且元素值唯一;
- set中的元素默认按照小于来比较;
- set中查找某个元素的时间复杂度为: O($\log_{2}$ N)


## 2. set的基本操作
```cpp
insert();    // 插入元素
count();     // 判断set中是否存在某个元素
size();      // 返回set中的元素个数
erase();     // 删除set中的某个元素
clear();     // 清空set中的所有元素
empty();     // 判断set是否为空
begin();     // 返回set第一个节点的迭代器
end();       // 返回set结尾节点+1的迭代器
rbegin();    // 反向迭代器,同end()
rend();      // 反向迭代器,同rbegin()

find();          // 在set中查找某个指定元素的位置
lower_bound();   // 二分查找第一个不小于某个值的元素的迭代器
get_allocator(); // 返回集合的分配器
swap();          // 交换两个集合的变量
max_size();      // 返回集合能够容纳元素的最大限值
```

## 3. 遍历set的方法
- 1 使用迭代器遍历
```cpp
#include <iostream>
#include <set>

int main() {
    std::set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);
    set<int>::iterator it;
    for (it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
```

- 2 使用foreach遍历
```cpp
#include <iostream>
#include <set>

int main() {
    std::set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);
    set<int>::iterator it;
    for (auto& num : s) {
        std::cout << num << " ";
    }

    return 0;
}
```

