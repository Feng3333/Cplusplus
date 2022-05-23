# 真的是随便记记，

为啥是随便记记呢？---------->>因为这些STL的容器或是算法都是我刷题或是看他人的代码发现了自己未知的领域一时兴起记录下来的，还没有即时进行分类，当然，本人是个弱鸡，还很懒，所以只能先暂时记录下来了，也不知道以后有没有时间可以完成分类，

## lower_bound( )和upper_bound( )的常见用法

### 一、在从小到大的排序数组中，

lower_bound( begin,end,num)：从数组的begin位置到end-1位置二分查找第一个大于或等于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。

upper_bound( begin,end,num)：从数组的begin位置到end-1位置二分查找第一个大于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。


### 二、在从大到小的排序数组中，重载lower_bound()和upper_bound()

`lower_bound( begin,end,num,greater<type>())`:从数组的begin位置到end-1位置二分查找第一个小于或等于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。

`upper_bound( begin,end,num,greater<type>())`:从数组的begin位置到end-1位置二分查找第一个小于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。
	
```c++
int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
    
    sort(arr2.begin(), arr2.end());
    int len = (int)arr2.size();
    int cnt = 0, index;
        
    for(int i : arr1) {
        index = lower_bound(arr2.begin(), arr2.end(), i) - arr2.begin();
        if(index >= 0 && index < len) {
            if(abs(arr2[index] - i) <= d) {
                continue;
            }
        }
        index--;
        if(index >= 0 && index < len) {
            if(abs(arr2[index] - i) <= d) {
                continue;
            }
        }
        cnt ++;
    }
    return cnt;
}
 ```
	
##  binary_search()函数详解

该函数有 2 种语法格式，分别为：
```c++
//查找 [first, last) 区域内是否包含 val
bool binary_search (ForwardIterator first, ForwardIterator last, const T& val);

//根据 comp 指定的规则，查找 [first, last) 区域内是否包含 val
bool binary_search (ForwardIterator first, ForwardIterator last, const T& val, Compare comp);
```

其中，first 和 last 都为正向迭代器，[first, last) 用于指定该函数的作用范围；val 用于指定要查找的目标值；comp 用于自定义查找规则，此参数可接收一个包含 2 个形参（第一个形参值为 val）且返回值为 bool 类型的函数，可以是普通函数，也可以是函数对象。
同时，该函数会返回一个 bool 类型值，如果 binary_search() 函数在 [first, last) 区域内成功找到和 val 相等的元素，则返回 true；反之则返回 false。

需要注意的是，由于 binary_search() 底层实现采用的是二分查找的方式，因此该函数仅适用于“已排好序”的序列。所谓“已排好序”，并不是要求 [first, last) 区域内的数据严格按照某个排序规则进行升序或降序排序，只要满足“所有令 element<val（或者 comp(val, element）成立的元素都位于不成立元素的前面（其中 element 为指定范围内的元素）”即可。

举例：
```c++
#include <iostream>     
#include <algorithm>    // std::binary_search
#include <vector>       
using namespace std;
//以普通函数的方式定义查找规则
bool mycomp(int i, int j) { return i > j; }
//以函数对象的形式定义查找规则
class mycomp2 {
public:
    bool operator()(const int& i, const int& j) {
        return i > j;
    }
};
int main() {
    int a[7] = { 1,2,3,4,5,6,7 };
    //从 a 数组中查找元素 4
    bool haselem = binary_search(a, a + 9, 4);
    cout << "haselem：" << haselem << endl;
    vector<int>myvector{ 4,5,3,1,2 };
    //从 myvector 容器查找元素 3
    bool haselem2 = binary_search(myvector.begin(), myvector.end(), 3, mycomp2());
    cout << "haselem2：" << haselem2;
    return 0;
}
```


##  max_element（）与min_element（）

####  max_element（）与min_element（）分别用来求最大元素和最小元素的位置。

接收参数：容器的首尾地址（迭代器）（可以是一个区间）

返回：最值元素的地址（迭代器），需要减去序列头以转换为下标

代码示例：
```c++
vector<int> n;
int maxPosition = max_element(n.begin(),n.end()) - n.begin(); //最大值下标

int minPosition = min_element(n.begin(),n.end()) - n.begin();//最小值下标

2）普通数组
int a[]={1,2,3,4};
int maxPosition = max_element(a,a+2) - a; //最大值下标

int minPosition = min_element(a,a+2) - a;//最小值下标
```

#### *max_element()与 *min_element() 分别用来求最大元素和最小元素的值。

接收参数：容器的首尾地址（迭代器）（可以是一个区间）

返回：最值元素的值

代码示例：
```c++
int maxValue = *max_element(n.begin(),n.end()); //最大值

int minValue = *min_element(n.begin(),n.end());//最小值

int maxValue = *max_element(a,a+2); //最大值

int minValue = *min_element(a,a+2);//最小值
```


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

## find( )

find() 函数本质上是一个模板函数，用于在指定范围内查找和目标元素值相等的第一个元素。

find() 函数的语法格式：
InputIterator find (InputIterator first, InputIterator last, const T& val);
其中，first 和 last 为输入迭代器，[first, last) 用于指定该函数的查找范围；val 为要查找的目标元素。
正因为 first 和 last 的类型为输入迭代器，因此该函数适用于所有的序列式容器。

#### 1、find()用于普通数组
```c++
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int arr[] = {1,2,3,4,5,6};
    int len = 6;
    
    if ( find(arr,arr+len,2) != arr+len ) cout << "查询成功" << endl;
    else  cout << "查询失败" << endl;
    
    return 0;
}
```

#### 2、find()用于容器
```c++
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main() {
    vector<int> arr = {1,2,3,4,5,6};
    int len = 6;
    
    if ( find(arr.begin(),arr.end(),2) != arr.end() ) cout << "查询成功" << endl;
    else  cout << "查询失败" << endl;
    
    return 0;
}
```

#### 3、find()用于字符串
```c++
#include <iostream> 
#include <cstring>
using namespace std;

int main() {
    string s = "Hello World";
    
    if (s.find("o") != string::npos ) cout << "查询成功" << endl;
    else cout << "查询失败" << endl;
    
    return 0;
}
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
