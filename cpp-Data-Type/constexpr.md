# constexpr
## 目录
 - [1. 基本介绍](#1-基本介绍)
 - [2. 常量表达式](#2-常量表达式)
 - [3. constexpr变量](#3-constexpr变量)
 - [4. constexpr函数](#4-constexpr函数)
 
## 1. 基本介绍
constexpr表达式是指值不会改变并且在编译过程就能得到计算结果的表达式.   
声明为constexpr的变量一定是一个const变量，而且必须用常量表达式初始化;
constexpr只能用于修饰简单的常量表达式，不包括string类型；

## 2. 常量表达式
常量表达式(const expression)： 指值不会改变且在编译过程中就能得到计算结果的表达式； 
举个例子：  
```c++
const int maxSize = 100;
const int limit = maxSize + 1;
```
反例：  
```c++
int staff_size = 20;
const int sz = get_size();
```
- staff_size 的初始值虽然是和字面常量值，但是它的数据类型只是普通的int而非const int ，还是可以被重新赋值的，所以不是常量表达式；
- sz本身是一个常量，但它的具体值直到运行时才可以获取到，所以也不是常量表达式；

## 3. constexpr变量
在一个复杂的系统中，很难分辨一个初始值是不是一个常量表达式，比如即使变量加上了const，但是赋值实在运行时确定的也不是常量表达式。

C++11的新规定中，允许将变量声明为constexpr类型以便由编译器来验证变量的值是否为常量表达式。
- 声明为constexpr的变量必须为一个常量。
- 必须用常量表达式初始化。
```cpp
constexpr int mf = 20;           // 20是常量表达式
constexpr int limit = mf + 1;    // mf + 1是常量表达式
const int sz = size();           // 只有当size是一个constexpr函数时才是一条正确的声明语句
```
size()函数也需要constexpr修饰，成为constexpr函数。

## 4. constexpr函数
constexpr函数指能用于常量表达式的函数，定义constexpr函数有几项约定：
- 函数的返回值类型及所有的类型都必须是 字面值类型 
- 函数体中必须有且只有一条return语句。
```cpp
constexpr int size() {return 30;}
constexpr int foo = size();        //foo就为一个常量表达式
```
因为编译器能在程序编译时验证size函数返回的是一个常量表达式，所以可以用size函数初始化constexpr类型的变量foo；

(1) 执行初始化任务时，编译器把对constexpr函数的调用替换成结果值。为了能在编译过程中随时展开，constexpr函数被隐式地指定为内联函数。  

(2) constexpr函数体也可以包含其他语句，只要这些语句在运行时不执行任何操作就行，例如：constexpr函数中可以有空语句，类型别名，using声明。

(3) constexpr函数的返回值可以不是一个常量：
```cpp
// cnt如果是常量表达式，返回值就是常量表达式
constexpr size_t scale(size_t cnt) {return new_sz() *cnt;}
```
比如,下面两个例子
```cpp
int arr[scale(2)];  // 正确：scale(2)是常量表达式
int i = 3;
int a[scale(i)];    // 错误：scale(i)不是常量表达式
```
- 给scale传入字面值2的常量表达式时，他的返回类型也是常量表达式，此时编译器用对应的结果值替换为对scale函数的调用
- 当用一个非常量表达式调用scale函数是，比如int i = 3 的对象 i ,返回值则不是一个常量表达式。当把scale函数用在其他需要常量表达式的上下文中时，编译器发现不是常量表达式，发出错误信息。 

(4) constexpr函数通常定义在头文件中，因为编译器想要展开函数不仅需要函数声明，还需要函数定义，而constexpr函数可以在程序中多次定义，但多个定义必须完全一致。
