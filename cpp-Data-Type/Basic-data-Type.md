# 基础数据类型

## C++命名规则
1. 在名称中只能使用字母字符、数字和下划线( \_ );  

2. 名称的第一个字符不能是数字；  

3. 区分大写字符和小写字符；  

4. 不能将 C++ 关键字用作名称；  

5. 以两个下划线或者下划线和大写字母打头的名称被保留给实现(编译器及其使用的资源)使用。以一个下划线开头的名称被保留给实现，用作全局标识符；  

6. C++ 对于名称的长度没有限制，名称中所有的字符都有意义，但有些平台有长度限制；  

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

### char类型
char可以用来存储各种字符（数字，字母，一些特殊符号);  

char为什么只占一个字节？ 
实际上很多系统支持的字符都不超过128个，因此用一个字节(8 bit)就可以表示所有的符号，char也算作是一种整型；    

字符集中的字符用数值编码(ASCII码)表示


### 浮点数
