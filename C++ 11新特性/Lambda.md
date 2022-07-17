# Lamdba表达式

Lamdba又被称为匿名表达式 (在句中定义和声明的一个没有名字的临时函数,只有在调用的时候才会创建函数对象);  
什么使用可以使用Lamdba表达式：  
函数不经常需要被使用，而且函数比较简单；  

Lamdba的本质使用一个特殊的，匿名的类类型，它是一个带operator()的类，即仿函数;  
仿函数(functor): 就是是一个类的使用看上去像一个函数，其实现就是类中实现一个operator(),这个类就有了类似函数的行为,即是一个仿函数类了.

举个栗子：
```c++
#include <iostream>

class AddNum {
public:
	//构造函数,初始化 num_
	AddNum(int num) :num_(num) {};

	int add(int x) const { //这里的const修饰的是this指针
		return x + num_;
	}
  
  //重载 ()运算符
	int operator()(int x) {
		return num_ + x;
	};
	
private:
	int num_;
};

int main() {
	//functor
	auto num1 = AddNum(10);
	auto num2 = num1(5);
	std::cout << "num2 : " << num2 << std::endl;

	//Lambda
	auto num3 = [curnum = 1](int x){
		return curnum + x;
	};
	auto num4 = num3(2);
	std::cout << "num4 : " << num4 << std::endl;
  
	return 0;
}
```

## Lambda表达式的基本构造

一个完整的Lambda表达式的的组成如下：
```
[capture-list]  (params) mutable(optional) exception(optional) attribute(optional) -> ret(optional) { body }   
```
1. capture-list : 捕获列表，Lambda可以把上下文变量以值或引用的方式捕获，在body中直接使用。  
2. params : 参数列表，和普通函数一样的参数，在c++14之后允许使用auto左右参数类型。  
3. mutable ：只有这个 Lambda 表达式是 mutable 的才允许修改按值捕获的参数。  
4. exception : 异常标识。  
5. attribute : 属性标识。  
6. ret : 返回值类型，可以省略，让编译器通过 return 语句自动推导。  
7. body : 函数的具体逻辑。  

### 官方文档的 Lambda 表达式写法:    
具体链接：https://en.cppreference.com/w/cpp/language/lambda
![image](https://github.com/Feng3333/Cplusplus/blob/990e74e994dfe31193ff58671ada5d6f23cce1a7/C++%2011%E6%96%B0%E7%89%B9%E6%80%A7/images-folder/Lambda1.png)

### capture-list 捕获列表

Lambda表达式与普通函数比较大的一个区别便是捕获列表;  

#### 常用的几种捕获列表的书写方式：  

[] 什么也不捕获，即空捕获列表，在lambda表达式内部无法使用其外层上下文中的任何局部名字，其数据需要从实参或者非局部变量中获得

[=] 按值的方式捕获所有变量

[&] 按引用的方式捕获所有变量

[=, &a] 除了变量a之外，按值的方式捕获所有局部变量，变量a使用引用的方式来捕获。这里可以按引用捕获多个，例如 [=, &a, &b,&c]。这里注意，如果前面加了=，后面加的具体的参数必须以引用的方式来捕获，否则会报错。

[&, a] 除了变量a之外，按引用的方式捕获所有局部变量，变量a使用值的方式来捕获。这里后面的参数也可以多个，例如 [&, a, b, c]。这里注意，如果前面加了&，后面加的具体的参数必须以值的方式来捕获。

[a, &b] 以值的方式捕获a，引用的方式捕获b，也可以捕获多个。

[this] 在成员函数中，也可以直接捕获this指针，其实在成员函数中，[=]和[&]也会捕获this指针。

#### 举个栗子
```cpp
#include <iostream>

int main()
{
    int a = 3, b = 5;

    //按值捕获
    auto function1 = [=](int x) {
        std::cout << "a + " << x << " = " << a + x << std::endl;
    };
    function1(2);

    //按值捕获
    auto function2 = [a](int x) {
        std::cout << "a + " << x << " = " << a + x << std::endl;
    };
    function2(3);

    //按引用捕获
    auto function3 = [&a](int x) {
        std::cout << "a + " << x << " = " << a + x << std::endl;
    };
    function3(4);

    //按引用捕获
    auto function4 = [&] {
        std::cout << "a = " << a << " and " << "b = " << b << std::endl;
    };
    function4(); 

    return 0;
}
```
