# bitset

## 目录
   - [基本介绍](#-基本介绍)

## 基本介绍 
头文件：
```c++
#include <bitset>
```
bitset就是比特集合，可用于位运算等操作;  

## 使用方法

```c++
bitset<n> num   // num有n位，默认全为0
bitset<n> num(x)  // 将数字x转换为二进制保存到num中，num有n位，实际位数不够时,用0填充，比如：00001110;
```

##  __builtin_popcount

\_\_builtin\_popcount为C++内置函数，无需添加头文件;  
作用：可以计算一个数二进制中1的个数
```cpp
int ans = __builtin_popcount(5);    // ans = 2
```
