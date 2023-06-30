# C++ STL中的算法总结
# 目录
 - [遍历](#遍历)
   - [for_each](#for_each)
 - [排序](#排序)
   - [sort()](#sort)
   - [stable_sort()](#stable_sort)
 - [查找](#查找)
   - [find()](#find)
   - [lower_bound和upper_bound](#lower_bound和upper_bound)
   - [binary_search()](#binary_search)
   - [max_element与min_element](#max_element与min_element)

# 遍历
## for_each
遍历容器
```
for_each(iterator begin, iterator end, _func);
```
其中 begin为迭代器开始的位置， end为迭代器结束的位置，_func为函数或者函数对象
代码示例：
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

void Print(int val) 
{
    std::cout << val << " ";
};

// 函数对象
class Print_c
{
public:
    void operator()(int val)
    {
        std::cout << val << " ";
    }
};

int main()
{   
    std::vector<int> nums(10);
    std::iota(nums.begin(), nums.end(), 0);
    for_each(nums.begin(), nums.end(), Print);
    std::cout << std::endl;
    std::cout << "使用函数对象的形式: ";
    for_each(nums.begin(), nums.end(), Print_c());
    return 0;
};
```

# 排序
### sort()
包含的头文件：
```cpp
#include <algorithm>
```
- sort(first, last)：对容器或普通数组中 \[first, last) 范围内的元素进行排序，默认进行升序排序。
- sort()函数是基于快速排序实现的，平均的时间复杂度为 N\*logN (底数为2);
- 对于指定区域内值相等的元素，sort() 函数无法保证它们的相对位置不发生改变;

#### 使用sort()函数时的一些限制:  
- 1. 容器支持的迭代器类型必须为随机访问迭代器。这意味着，sort() 只对 array、vector、deque 这 3 个容器提供支持。
- 2. 如果对容器中指定区域的元素做默认升序排序，则元素类型必须支持 < 小于运算符；同样，如果选用标准库提供的其它排序规则，元素类型也必须支持该规则底层实现所用的比较运算符 
- 3. sort() 函数在实现排序时，需要交换容器中元素的存储位置。这种情况下，如果容器中存储的是自定义的类对象，则该类的内部必须提供移动构造函数和移动赋值运算符。

### stable_sort()
包含的头文件：
```cpp
#include <algorithm>
```
- stable_sort() 和 sort() 具有相同的使用场景，就连语法格式也是相同的;
- stable_sort() 与 sort() 的主要区别在于：在功能上除了可以实现排序，还可以保证不改变相等元素的相对位置；
- stable_sort() 是基于归并排序实现的;
- 当可用空间足够的情况下，该函数的时间复杂度可达到O(N\*log2(N))；反之，时间复杂度为O(N\*log2(N)2);


# 查找
其中 begin为迭代器开始的位置， end为迭代器结束的位置，_func为函数或者函数对象 代码示例：

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

void Print(int val) 

## find()
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

## lower_bound()和upper_bound()

### 一、在从小到大的排序数组中，

lower_bound( begin,end,num)：从数组的begin位置到end-1位置二分查找第一个大于或等于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。
max_element()与min_element()
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

##  binary_search()

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

##  max_element与min_element
####  max_element（）与min_element（）分别用来求最大元素和最小元素的位置。

接收参数：容器的首尾地址（迭代器）（可以是一个区间）

返回：最值元素的地址（迭代器），需要减去序列头以转换为下标

代码示例：
```c++
vector<int> n;
int maxPosition = max_element(n.begin(),n.end()) - n.begin()); //最大值下标

int minPosition = min_element(n.begin(),n.end()) - n.begin());//最小值下标

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
