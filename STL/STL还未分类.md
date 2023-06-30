# 真的是随便记记，

为啥是随便记记呢？---------->>因为这些STL的容器或是算法都是我刷题或是看他人的代码发现了自己未知的领域一时兴起记录下来的，还没有即时进行分类，当然，本人是个弱鸡，还很懒，所以只能先暂时记录下来了，也不知道以后有没有时间可以完成分类，
##  accumulate的用法

###  1.累加求和

举例：
```c++
//vec为某个数组，num为初始的累加值
int sum = accumulate(vec.begin() , vec.end() , num); 
```

另外 ：可以使用accumulate把string型的vector容器中的元素连接起来：
```c+
string sum = accumulate(v.begin() , v.end() , string(" "));
//这个函数调用的效果是：从空字符串开始，把vec里的每个元素连接成一个字符串
```

### 2.自定义的数据类型的处理

C++ STL中有一个通用的数值类型计算函数— accumulate(),可以用来直接计算数组或者容器中C++内置数据类型，例如:
```c++
#include <numeric> //调用aaccumulate()所需的头文件

int arr[]={10,20,30,40,50};
vector<int> va(&arr[0],&arr[5]);
int sum=accumulate(va.begin(),va.end(),0); //sum = 150
```

但是对于自定义数据类型，我们就需要自己动手写一个回调函数来实现自定义数据的处理，然后让它作为accumulate()的第四个参数，accumulate()的原型为:
```c++
template<class _InIt,
class _Ty,
class _Fn2> inline
_Ty _Accumulate(_InIt _First, _InIt _Last, _Ty _Val, _Fn2 _Func)
{    /利用_Func来计算_First和_Last之间的和, 
for (; _First != _Last; ++_First)
_Val = _Func(_Val, *_First);
return (_Val);
}
```

具体举例说明：
```c++
#include <vector>
#include <string>

using namespace std;

struct Grade {
    string name;
    int grade;
};

int main() {
    Grade subject[3] = {
        { "English", 80 },
        { "Biology", 70 },
        { "History", 90 }
    };

    int sum = accumulate(subject, subject + 3, 0, [](int a, Grade b){return a + b.grade; });
    cout << sum << endl;

    system("pause");
    return 0;
}
```

## __gcd( )

```c++
//求两个数的最大公约数,其中a，b的数据类型要相同
    int a,b;
    int ans = __gcd(a,b);
```

## iota()

iota函数可以对一个范围数据进行赋值：
```c++
//iota()内部实现原理
template <class ForwardIterator, class T>
  void iota (ForwardIterator first, ForwardIterator last, T val)
{
  while (first!=last) {
    *first = val;
    ++first;
    ++val;
  }
}
```

具体用法:
```c++
// iota example
#include <iostream>     // std::cout
#include <numeric>      // std::iota
#include <vector>      
int main () {
  int numbers[10];
  vector<int> nums(100);
  
  std::iota (numbers,numbers+10,100);
  std::iota (nums.begin(),nums.end(),0); //其中0为初始值num[0]的数值
  
  std::cout << "numbers:";
  for (int& i:numbers) std::cout << ' ' << i;
  //这里打印出来的数组为:numbers:100 101 102 103 104 105 106 107 108 109 
  std::cout << '\n';
  return 0;
}
```


## partition()与stable_partition()

### partition()
partition算法作用为对指定范围内元素重新排序，使用输入的函数，把结果为true的元素放在结果为false的元素之前  
一般用法:
```c++
partition(vector<int>::iterator.first,vector<int>::iterator.last,cmp)
/*first 和 last 为正向迭代器，其组合 [first, last) 用于指定该函数的作用范围；cmp用于指定筛选规则。
（筛选规则，其本质就是一个可接收 1 个参数且返回值类型为 bool 的函数，可以是普通函数，也可以是一个函数对象。）
同时，partition() 函数还会返回一个正向迭代器，其指向的是两部分数据的分界位置，更确切地说，指向的是第二组数据中的第 1 个元素。*/
```
示例：
```c++
#include <iostream>     
#include <algorithm>   
#include <vector>       
using namespace std;

//以普通函数的方式定义partition()函数的筛选规则
bool cmp(int i) { return (i % 2) == 0; }

//以函数对象的形式定义筛选规则
class cmp2{
public:
    bool operator()(const int& i) {
        return (i%2 == 0);
    }
};

int main() {
    vector<int> myvector(9);
    //对myvector中的值依次进行赋值
    std::iota (myvector.begin(), myvector.end(),1);
    
    vector<int>::iterator bound;
    //以 mycomp2 规则，对 myvector 容器中的数据进行分组
    bound = std::partition(myvector.begin(), myvector.end(), cmp2());
    for (vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\nbound = " << *bound;
    return 0;
}
```
运行结果：  
8 2 6 4 5 3 7 1 9  
bound =5

### stable_partition()函数
相较于partition()的只负责对指定区域内的数据进行分组（不保证各组中元素的相对位置不发生改变）;    
stable_partition() 函数可以保证对指定区域内数据完成分组的同时，不改变各组内元素的相对位置。   

还是以上面所创建的myvector为例:
```c++
    bound = std::stable_partition(myvector.begin(),myvector.end(),[](const & a){
        return a%2 == 0;
    });
    for (auto num : myvector) cout << num << endl;
    cout << "\nbound = " << *bound;
```
此时的运行结果就是有明显的顺序感：  
2 4 6 8 1 3 5 7 9  
bound = 1

## nth_element 求区间第k小的数

nth_element(first, nth, last, compare)  
求[first, last]这个区间中第n大小的元素，如果参数加入了compare函数，就按compare函数的方式比较。  

nth_element仅排序第n个元素（从0开始索引），即将位置n（从0开始）的元素放在第n大的位置，处理完之后，默认排在它前面的元素都不比它大，排在它后面的元素都不比它小。  

例如：array[first, last)元素区间，排序后，array[nth]就是第n大的元素（从0开始）  
但是[first, nth) 和 [nth,last)区间的大小顺序不一定。可以确定的是array[nth]一定是整个区间里第n大的元素。  
[first,nth)中的元素都是不大于array[nth]的，[nth, last)中的元素都是不小于array[nth]的。  

具体用法：
```c++
#include<iostream>
#include<algorithm>

using namespace std;
 
int main(){
	int array[] = {5,6,7,8,4,2,1,3,0};
	
	//sizeof (array)就是这个数组所占的内存总量（字节数),
        //sizeof(int)就是单个所占的内存.
	//sizeof (array） / sizeof (int)就是array数组的元素的个数.
	int len=sizeof(array)/sizeof(int);
	
	cout<<"排序前: ";
	for(int i=0; i<len; i++)
		cout<<array[i]<<" ";
	nth_element(array, array+6, array+len);  //排序第6个元素
		cout<<endl;
	cout<<"排序后："; 
	for(int i=0; i<len; i++)
		cout<<array[i]<<" ";
		
	cout<<endl<<"第6个元素为"<<array[6]<<endl; 
}
```
运行结果：
```
排序前：5 6 7 8 4 2 1 3 0
排序后：4 0 3 1 2 5 6 7 8
第六个元素是 6;
```

## std::reduce
需包含的头文件:
```c++
#include <numeric>
```
操作: 将某个区间内的元素按照指定方式求值;  
- 与std::accumulate有点像，两者的区别是:  
accmumlate一般是按照顺序处理元素并进行操作的, 但reduce并不是依次执行的;  
两者的运行效率在不同的情况下是不太一样的，比如处理int型时一般是accumulate更优, 处理double型时一般是reduce更优;  

- ps: 目前由于编译器和本地ide的原因，在vscode上使用std::reduce报错 ( 说是没有定义该函数, 但是我看了numeric里面确实已经定义了 ) , 无法进行一系列的验证和测试... google了一下也没有比较好的解决方法...  
