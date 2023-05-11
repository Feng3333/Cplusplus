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
