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
