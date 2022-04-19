# 真的是随便记记，

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
