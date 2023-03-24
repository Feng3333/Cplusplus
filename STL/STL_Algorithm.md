# C++ STL中的算法总结
## 目录
  - [sort()](#sort)
  - [stable_sort()](#stable_sort)

## sort()
包含的头文件：
```cpp
#include <algorithm>
```
- sort(first, last)：对容器或普通数组中 \[first, last) 范围内的元素进行排序，默认进行升序排序。
- sort()函数是基于快速排序实现的，平均的时间复杂度为 N\*logN (底数为2);
- 对于指定区域内值相等的元素，sort() 函数无法保证它们的相对位置不发生改变;

#### 使用sort()函数时的一些限制:  
- 1. 容器支持的迭代器类型必须为随机访问迭代器。这意味着，sort() 只对 array、vector、deque 这 3 个容器提供支持。
- 2. 如果对容器中指定区域的元素做默认升序排序，则元素类型必须支持 < 小于运算符；同样，如果选用标准库提供的其它排序规则，元素类型也必须支持该规则底层实现所用的比较运算符 
- 3. sort() 函数在实现排序时，需要交换容器中元素的存储位置。这种情况下，如果容器中存储的是自定义的类对象，则该类的内部必须提供移动构造函数和移动赋值运算符。

## stable_sort()
包含的头文件：
```cpp
#include <algorithm>
```
- stable_sort() 和 sort() 具有相同的使用场景，就连语法格式也是相同的;
- stable_sort() 与 sort() 的主要区别在于：在功能上除了可以实现排序，还可以保证不改变相等元素的相对位置；
- stable_sort() 是基于归并排序实现的;
- 当可用空间足够的情况下，该函数的时间复杂度可达到O(N\*log2(N))；反之，时间复杂度为O(N\*log2(N)2);