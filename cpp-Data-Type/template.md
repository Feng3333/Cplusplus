# template
## 目录
 - [1. template的类模板的使用](#1-template的类模板的使用)
   - [1.1 类模板的简单创建](#11-类模板的简单创建)
   - [1.2 类模板的具体使用方法](#12-类模板的具体使用方法)
   - [1.3 声明和使用类模板的一般方法](#13-声明和使用类模板的一般方法)
   - [1.4 使用类模板时需要注意的点](#14-使用类模板时需要注意的点)


## 1. template的类模板的使用
### ----->C++类模板 template \<class T>详细使用方法:  
#### 1.1 类模板的简单创建
举个例子: 有两个或多个类，其功能是相同的，仅仅是数据类型不同，如下面语句声明了一个类:
```cpp
class Compare_int
{
public :
   Compare(int a,int b)
   {
      x=a;
      y=b;
   }
   int max( )
   {
      return (x>y)?x:y;
   }
   int min( )
   {
      return (x<y)?x:y;
   }
private :
   int x,y;
};
```
此时如果需要比较两个double类型的数时就需要重新创造一个类:  
```cpp
class Compare_float
{
public :
   Compare(float a,float b)
   {
      x=a;y=b;
   }
   float max( )
   {
      return (x>y)?x:y;
   }
   float min( )
   {
      return (x<y)?x:y;
   }
private :
   float x,y;
}
```
显然这基本上是重复性的工作，应该有办法减少重复的工作.   
C++在发展的后期增加了 模板(template ) 的功能，提供了解决这类问题的途径.可以声明一个通用的类模板，它可以有一个或多个虚拟的类型参数，如对以上两个类可以综合写出以下的类模板：  
```cpp
template <class numType> //声明一个模板，虚拟类型名为numtype
class Compare
{
public:
  Compare(numType a, numType b) {
    x = a;
    y = b;
  }
  
  numType max() {
    return x > y ? x : y;
  }
  
  numType min() {
    return x < y ? x : y;
  }
private:
  numType x, y;
}
```
由于类模板包含类型参数，因此又称为参数化的类.如果说类是对象的抽象，对象是类的实例，则类模板是类的抽象，类是类模板的实例. 利用类模板可以建立含各种数据类型的类;  

#### 1.2 类模板的具体使用方法
上述用普通的类来定义对象的方法:    
```cpp
    Compare_int cmp1(4,7); // Compare_int是已声明的类
```
其作用是建立一个Compare_int类的对象,并将实参4和7分别赋给形参a和b,作为进 行比较的两个整数. 

用类模板定义对象的方法与此相似,但是不能直接写成  
```cpp
 Compare cmp(4,7); // Compare是类模板名
```
Compare是类模板名,而不是一个具体的类,类模板体中的类型numtype并不是一个实际的类型,只是一个虚拟的类型,无法用它去定义对象.必须用实际类型名去取代虚拟的类型，具体的做法是:   
```cpp
  Compare <int> cmp(4,7);
```
即在类模板名之后在尖括号内指定实际的类型,在进行编译时,编译系统就用int取代类模板中的类型参数numtype,
这样就把类模板具体化了,或者说实例化了.这时Compare\<int>就相当于前面介绍的Compare_int类
  
继续上述的例子:  
```cpp
#include <iostream>

template <class numType>
class Compare
{
public:
    Compare(numType a, numType b) {
        x = a;
        y = b;
    }
    numType max() {
        return x > y ? x : y;
    }
    numType min() {
        return x < y ? x : y;
    }

private:
    numType x, y;
};

int main() {
    Compare<int> cmp1(2,3);
    std::cout <<"cmp1.max is: " << cmp1.max() << std::endl;
    Compare<char>cmp2('f', 'w');
    std::cout <<"cmp2.min is: " << cmp2.min() << std::endl;
    return 0;
}
```
还有一个问题要说明:  上面列出的类模板中的成员函数是在类模板内定义的.如果改为在类模板外定义,不能用一般定义类成员函数的形式:  
```cpp
numtype Compare::max( ) {…} //不能这样定义类模板中的成员函数
```
而应当写成类模板的形式:
```cpp
    template <class numtype>
    numtype Compare<numtype>::max( )
    {
        return (x>y)?x:y;
    }
```
上面第一行表示是类模板，第二行左端的numtype是虚拟类型名，后面的Compare \<numtype>是一个整体，是带参的类。表示所定义的max函数是在类Compare \<numtype>的作用域内的。在定义对象时，用户当然要指定实际的类型（如int），进行编译时就会将类模板中的虚拟类型名numtype全部用实际的类型代替。这样Compare \<numtype >就相当于一个实际的类;

#### 1.3 声明和使用类模板的一般方法
1) 先写出一个实际的类,(由于语义明确, 含义清楚, 一般不会出错);  
2) 将此类中需要准备改变的类型名(如int需要改成double或者char等等)切换成一个自己指定的虚拟类型名(T);  
3) 在类声明前面加入一行:  
```cpp
template <class T> //T为虚拟参数类型
```
4) 使用类模板定义对象时需要注意格式:  
```cpp
//类模板名<实际类型名> 对象名;
Class_1<int> c1;
//类模板名<实际类型名> 对象名(参数列表);
Class_1<char> c2('f','w');
```
5) 如果在类模板外定义成员函数,则应写成类模板形式:  
```cpp
template <class 虚拟类型参数>
函数类型 类模板名<虚拟类型参数>::成员函数名(函数形参表列) {…}
```

#### 1.4 使用类模板时需要注意的点
1) 类模板的类型参数可以有一个或者多个, 每个类型前面都必须加class:  
```cpp
template <class T1, class T2>
class className {...};
```
在定义对象时科分别带入实际的类型名
```cpp
class <double, int> obj1;
```
2) 和使用类一样, 使用类模板时要注意其作用域, 只能在其有效作用域内用它定义对象.  
3) 模板可以有层次, 一个类模板可以作为基类, 派生出派生模板类


