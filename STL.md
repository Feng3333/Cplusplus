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
	
## map和unordered_map的差别
    
### 1. 需要引入的头文件不同
map: #include < map >
unordered_map: #include < unordered_map >

### 2. 底层实现的数据结构不同

map是基于红黑树结构实现的。红黑树是一种平衡二叉查找树的变体结构，它的左右子树的高度差有可能会大于 1。所以红黑树不是严格意义上的平衡二叉树AVL，但对之进行平衡的代价相对于AVL较低， 其平均统计性能要强于AVL。红黑树具有自动排序的功能，因此它使得map也具有按键（key）排序的功能，因此在map中的元素排列都是有序的。在map中，红黑树的每个节点就代表一个元素，因此实现对map的增删改查，也就是相当于对红黑树的操作。对于这些操作的复杂度都为O(logn)，复杂度即为红黑树的高度。
    
unordered_map是基于哈希表（也叫散列表）实现的。散列表是根据关键码值而直接进行访问的数据结构。也就是说，它通过把关键码值映射到表中一个位置来访问记录，以加快查找的速度。这个映射函数叫做散列函数，存放记录的数组叫做散列表。散列表使得unordered_map的插入和查询速度接近于O(1)（在没有冲突的情况下），但是其内部元素的排列顺序是无序的。

### 3. 元素排列顺序不同
    
map：基于红黑树，元素有序存储；
unordered_map：基于散列表，元素无序存储；
    
### 4. 插入和查询的时间复杂度不同

map：基于红黑树，复杂度与树高相同，即O(logn)；
unordered_map：基于散列表，复杂度依赖于散列函数产生的冲突多少，但大多数情况下其复杂度接近于O(1)。
    
### 5. 效率及其稳定性不同
    
存储空间：unordered_map的散列空间会存在部分未被使用的位置，所以其内存效率不是100%的。而map的红黑树的内存效率接近于100%；
     
查找性能的稳定性：map的查找类似于平衡二叉树的查找，其性能十分稳定。例如在1M数据中查找一个元素，需要多少次比较呢？20次。map的查找次数几乎与存储数据的分布与大小无关。而unordered_map依赖于散列表，如果哈希函数映射的关键码出现的冲突过多，则最坏时间复杂度可以达到是O(n)。因此unordered_map的查找次数是与存储数据的分布与大小有密切关系的，它的效率是不稳定的。
    
### 6.优缺点及适用场景
    
#### `map`
	
优点：
map元素有序（这是map最大的优点，其元素的有序性在很多应用中都会简化很多的操作）
其红黑树的结构使得map的很多操作都可在O(logn)下完成；
map的各项性能较为稳定，与元素插入顺序无关；
map支持范围查找。
    
缺点：
占用的空间大：红黑树的每一个节点需要保存其父节点位置、孩子节点位置及红/黑性质，因此每一个节点占用空间大。
查询平均时间不如unordered_map。
    
适用场景：
元素需要有序；
对于单次查询时间较为敏感，必须保持查询性能的稳定性，比如实时应用等等。
          
#### `unordered_map`
    
优点：
查询速度快，平均性能接近于常数时间O(1)；
    
缺点：
元素无序； 
unordered_map相对于map空间占用更大，且其利用率不高；
查询性能不太稳定，最坏时间复杂度可达到O(n)。
    
适用场景：
要求查找速率快，且对单次查询性能要求不敏感。


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

