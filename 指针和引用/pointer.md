# 指针
## 指针的基本介绍

指针是一个变量，其存储的是值的地址，而不是值本身; 可以认为指针其实就相当于地址; 指针是一个其值为地址的变量.  
所以，看到指针的时候，首先想到的不是当前指针它指向哪里，而是应该想到这个变量存放的是一个地址，这个地址指向的是哪里.  (比如,char类型的变量中存放的是char类型的数据.int变量中存放的是int类型的数据.指针中存放的是一个地址!!!)

使用 & 地址运算符可以获取变量的地址;  [ 两个基本的运算符:&(取址运算符)和*(间接访问运算符/解引用指针) ]  

举个例子:  
```c++
#include <iostream>

using namespace std;

int main(){
  int num1 = 6;
  double num2 = 4.5;
  
  // &(取址运算符)
  cout << "num1: " << num1 << " address: " << &num1 << endl;
  cout << "num2: " << num2 << " address: " << &num2 << endl;
  
  // 对于 * 运算符 : 后面跟一个指针的时候,得到指针指向的内存中的内容.
  cout << "num1: " << *(&num1) <<endl; 
  
  return 0;
}
```

## 指针的声明和初始化

声明指针必须先指定指针指向的数据的类型！！
```cpp
// 基本模板-------> : 指向地址的数据类型 * 指针变量名
int* p1;
char* p2;
int* p3, p4 //声明了一个int型指针p3，int型数据p4------->对于每个指针变量名，都需要使用一个*
```  

举个栗子:  
```c++
#include <iostream>

using namespace std;

int main() {
	int a = 4, b = 5, c = 6, d = 7;
	double pi = 3.14159;

	//单独赋值，
	int *p_a = &a;
	double* p_pi = &pi;
	//多个赋值,
	int *p_b = &b, *p_c = &c, *p_d = &d, temp = 33;

	cout << "address of a is : " << p_a << endl;
	cout << "address of b is : " << p_b << endl;
	cout << "address of c is : " << p_c << endl;
	cout << "address of d is : " << p_d << endl; 
	cout << "address of pi is : " << p_pi << endl;

	return 0;
}
```
只要是出现了星号,而不管中间是不是有空格,便可以认为这算是创建了一个指针变量


## 初步使用指针
#### 指针的初始化
创建指针可以初始化也可以不初始化 (某些操作是需要考虑指针是否初始化的问题的)  

举个栗子:
```c++
#include <iostream>

using namespace std;

int main() {
	int num = 33;
	int* p1, * p2;

	//这里涉及编译器和IDE的原因，新版的C++指针没有初始化会报错
	/*cout << "p1, p2没有初始化时：" << endl;
	cout << "p1 : " << p1 << endl;
	cout << "p2 : " << p2 << endl;*/

	int* p3 = &num;
	cout << "初始化p3时：" << endl;
	cout << "p3 : " << p3 << endl;


	// 同类型的指针可以赋值；没有初始化的指针也可以被赋值
	p1 = p3;
	p2 = &num;
	cout << "p1, p2被赋值之后：" << endl;
	cout << "p1 : " << p1 << endl;
	cout << "p2 : " << p2 << endl;

	return 0;
}
```
