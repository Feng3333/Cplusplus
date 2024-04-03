![image](https://github.com/Feng3333/Cplusplus/assets/82488197/3ae62a49-ef24-4471-8f56-65a8260b14dd)# 基础数据类型
## 目录
- [C++命名规则](#c命名规则)
- [简单变量](#简单变量)
- [实际使用中?](#实际使用中)
- 
## C++命名规则
1. 在名称中只能使用字母字符、数字和下划线( \_ );  

2. 名称的第一个字符不能是数字；  

3. 区分大写字符和小写字符；  

4. 不能将 C++ 关键字用作名称；  

5. 以两个下划线或者下划线和大写字母打头的名称被保留给实现(编译器及其使用的资源)使用。以一个下划线开头的名称被保留给实现，用作全局标识符；  

6. C++ 对于名称的长度没有限制，名称中所有的字符都有意义，但有些平台会有长度限制；  

## 简单变量

### 整型

```cpp
short // short 至少16位

int //至少与short一样长

long //至少32位，且至少与int一样长

long long //至少64位，且至少与long一样长
```

可以使用sizeof来查看某个数据类型的长度（单位字节）
```cpp
int num = 12;
std::cout << "num's(int) bytes is : " << sizeof(num) << std::endl;
```

整型的 cout ?
```cpp
  int num = 9999;
  std::cout << std::dec;//十进制
  std::cout << "num is(十进制): " << num << std::endl;

  std::cout << std::hex; //十六进制
  std::cout << "num is(十六进制): " << num << std::endl;

  std::cout << std::oct; //八进制
  std::cout << "num is(八进制): " << num << std::endl;
```

### 无符号类型
使用关键字 unsigned 来修改声明:
```cpp
unsigned short u_s;
unsigned int u_i;
unsigned long u_l;
unsigned long long u_ll;
unsigned xx; //默认unsigned int
```
### bool 类型
C++ 中任何数字值或者指针都可以被隐式转换(即不用显示强制转换)成 bool 值，任何非 0 值都被转换成 true ，而 0 会转换成 false;

### char类型
char可以用来存储各种字符（数字，字母，一些特殊符号);  

char为什么只占一个字节？ 
实际上很多系统支持的字符都不超过128个，因此用一个字节(8 bit)就可以表示所有的符号，char也算作是一种整型；    

字符集中的字符用数值编码(ASCII码)表示


### 浮点数

#### 表示方式
C++有两种书写浮点数的方式:  

第一种是常用的标准小数点表示法:
```
12.12;
908873.23;
0.000123;
8.0;
```
第二种方法被称为 E表示法：  
比如 3.45E6 -----> 3450000 ；其中 E 后面的 6 称为指数，3.45称为尾数  
```
2.52e+8  //252000000
8.33E-4 //0.000833
7E3 //7000
9.11e-31 //0.0000000000000000000000000000000000000000911
```
E表示法适用于表示非常大的数或者非常小的数
指数位置的正负号，可以看做尾数小数点的右移(+)或者左移(-)----->浮点数的由来：小数点可移动

#### 浮点类型
```
float 至少32位，通常32位 : 有效数字为 6-7 ，数值范围为     -3.4E+38～3.4E+38
double 至少48位，通常64位:  有效数字为 15-16 ，数值范围为     -1.7E-308～1.7E+308
long double 至少64位，通常80、96、128位
```
三者的指数范围至少是 -37 ~ 37

需要注意的点：
使用cout输出时回删除结尾的0 :  
```
double num = 3.3333300000;
std::cout << num << std::endl;
printf(" %f" ,num);
```

#### 浮点常量
程序在存储浮点常量的时候默认是double类型;  
```c++
1.2345f //float类型
2.34E20F //float类型
1.342E28 //默认double类型
2.3L //long double类型

// 编译器可取到的double类型的最小值：
double minDouble = std::numeric_limits<double>::epsilon();

```

#### 浮点数的优缺点
优点：可以表示整数之间的值；  有缩放因子，表示的范围比较大；    
缺点：浮点的运算的速度比整数运算慢，且精度将降低；  
举个例子：  
```cpp
#include <iostream>

int main() {
  float a = 0.1;
  float b = 0.2;
  
  std::cout << "b+a = 0.3 ? " << (b+a == 0.3) <<std::endl;
  
  float c = 0.5;
  float d = 0.75;
  std::cout << "c+d = 1.25 ? " << (c+d == 1.25) << std::endl;
  return 0;
}

```
### 实际使用中？

```c++
#include <bits/types.h>
#include <cstdint>

typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;
```
历史包袱??
举个例子：
```c++
typedef unsigned int int32_t;

```
为了程序的可扩展性, 假如将来需要的数据⼤⼩变成了64bit时,只需要将typedef long long size_t就可以了, 不然可能需要重新修改很多地方;  
这种设计同样可以应⽤到⾃⼰的开发中来,当⾃⼰设计⼀个int类型保存某种数据时,但⼜没把握将来是不是要⽤long int时，就可以引用自己定义的一个数据类型；  

在不确定实际的某种整型的大小时，可以使用size_t:
```c++
for (size_t i = 0; i < vec.size(); ++i){};
```
需要注意的是，使用 size_t 类型来执行递减时， 如果 size_t num = -1 时，编译器会默认将 i 置为机器所可以接受的最大的整型，所以需要避免size_t类型的数小于 0 的情况出现

### 避免科学计数法
```c++
std::cout << std::fixed << num << std::endl;  //当num足够大时
```
