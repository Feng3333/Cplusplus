# 指针
## 目录
 - [1.0 指针的基本介绍](#1-指针的基本介绍)
 - [2.0 指针的声明和初始化](#2-指针的声明和初始化)
 - [3.0 初步使用指针](#3-初步使用指针)
    - [3.1 NULL指针 和 void\*](#31-null指针-和-void)

## 1. 指针的基本介绍

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

## 2. 指针的声明和初始化

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


## 3. 初步使用指针
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
注意 : 至少在解引用指针的时候,需要保证指针被正确的初始化或者正确的被赋过某个地址.不然,那样的解引用指针操作无意义且危险!!

### 3.1 NULL指针 和 void*
#### NULL指针
NULL指针着是一种非常特殊的指针，不指向任何东西，即表示不指向任何东西的指针,当然也不能够解引用;  

NULL指针的转换： 赋给这个指针一个0就行了.不用转化,就是整形的0.同样,直接赋NULL也是行的;  
举个栗子:  
```c++
#include <iostream>

using namespace std;

int main() {
	// 验证 NULL == 0 
	if (NULL == 0) {
		cout << "NULL == 0 ? " << "Yes" << endl;
	}

	int* p_a = 0;
	int* p_b = NULL;
	int num = 10;
	int* p_c = &num;

	cout << "p_a == 0 ? " << (p_a == 0) << endl;
	cout << "p_b == 0 ? " << (p_b == 0) << endl;
	cout << "p_c == 0 ? " << (p_c == 0) << endl;
	
	// 这里NULL指针解引用会报错
	//cout << "the address of p_a is " << *p_a << endl;
	
	return 0;
}
```

由于指针存放的是一个地址.一般来说,地址是整形没错,但是它是一种新的类型来表示地址.和整形并不能够兼容或者运算.但是当使用0来表示空指针的时候,0到底是整形常量还是一个指针常量?  
因此,在C++11中,新引入了一种特殊类型的字面值nullptr来初始化指针为空指针.他能够被转换成任何类型的指针;
```c++
#include <iostream>

using namespace std;

int main() {
	
	// 验证nullptr == 0;
	if (nullptr == 0) {
		cout << "nullptr == 0" << endl;
	}

	int num = 10;
	int* p = nullptr;
	p = &num;
	cout << "the address of num is : " << p << endl;

	return 0;
}
```
基本总结 : 尽量定义了对象之后再定义指向这个对象的指针,对于不清楚的指向哪里的指针,最好一律初始化为nullptr(C++11)或者NULL(0).之后再判断是否指向对象再进行相应的操作;  

#### void*
Void\* 是一种特殊类型的指针,能够用来存放任何类型对象的地址.通俗来说,就是不知道这个指针指向的是什么类型的对象.  
举个栗子:  
```c++
#include <iostream>

using namespace std;

int main() {
	
	double num = 25.4;
	//普通指针要保证类型相同
	double* p_a = &num;

	//void* 可以接受任何类型的指针
	void* p_b = &num;
	void* p_c = p_a;

	cout << "p_b : " << p_b << endl;
	cout << "p_c : " << p_c << endl;

	return 0;
}
```

### 指针的指针
