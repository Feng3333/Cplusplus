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

