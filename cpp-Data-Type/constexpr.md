# constexpr
## 目录
 - [1. 基本介绍](#1-基本介绍)
 - [2. 常量表达式](#2-常量表达式)
 - [3. constexpr变量](#3-constexpr变量)
 - [4. constexpr函数](#4-constexpr函数)
 - [5. 字面值类型](#5-字面值类型)
 - [6. 指针和constexpr](#6-指针和constexpr)
 - [7. 字面值常量类](#7-字面值常量类)
 
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
size()函数也需要co指针和constexprnstexpr修饰，成为constexpr函数。

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

## 5. 字面值类型
常量表达式的值需要在编译的时候就得到计算，因此对声明constexpr是用到的类型必须有所限制。因为这些类型一般比较简单，值也显而易见、容易得到，称之为“字面值类型”(literal type)

字面值类型包括：算数类型、引用、指针；   
而自定义类、string等类型不是字面值类型，也就不能定义成constexpr。

尽管指针和引用都能定义成constexpr，但是它们的初始值都受到严格的限制。一个constexpr指针的初始值必须是 nullptr 或者 0 ，或者是存储在某个固定地址中的对象。

函数体内定义的变量一般来说并非存放到固定地址中，因此constexpr指针不能指向这样的变量。相反的，定义在函数体之外的对象地址固定不变，能用来初始化constexpr指针。

## 6. 指针和constexpr
(1) 如果在constexpr声明中定义了一个指针，限定符constexpr仅对指针有效，与指针所指的对象无关。
```cpp
const int *p = nullptr;       // p是一个指向整数常量的指针
constexpr int *q = nullptr;   // q是一个指向整数的指针常量
```
q是一个指针常量，因为constexpr把它所定义的对象置为了顶层const。类似于: int \*const q = nullptr;

(2) 与其他指针常量类似，constexpr指针既可以指向常量也可以指向一个非常量：
```cpp
constexpr int *np = nullptr;     // np是一个指向整数的常量指针，其值为空
int j = 0;
constexpr int i = 40;            // i的类型为整数常量
constexpr const int *p = &i;     // p是指针常量，指向整数常量
constexpr int *p1 = &j;          // p1是指针常量，地址不可修改，指向整数j
```

## 7. 字面值常量类
constexpr函数的参数和返回值必须是字面值类型。注意，函数的返回值必须是字面值类型，但可以不是一个常量。  
和其他类不同，字面值类型的类可能含有constexpr函数成员。这样的成员必须符合constexpr函数的所有要求，他们是隐式const。  

字面值常量类： 数据成员都是字面值类型的聚合类。如果一个类不是聚合类，但它符合下述要求，则它也是一个字面值常量类：
- 数据成员都必须是字面值类型。
- 类必须至少含有一个constexpr构造函数。
- 如果一个数据成员含有类内初始值，这内置类型成员的初始值必须是一条常量表达式；如果成员属于某种类类型，这初始值必须使用成员自己的constexpr构造函数。
- 类必须使用析构函数的默认定义，该成员负责销毁类的对象。

尽管构造函数不能是const的，但是字面值常量类的构造函数可以是constexpr函数。一个字面值常量类必须至少提供一个constexpr构造函数。
