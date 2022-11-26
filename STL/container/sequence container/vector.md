# vector的基本介绍和相关操作

## 目录
- [1. 实现原理](#1-实现原理)  
  - [1.1 新增元素](#11-新增元素)
  - [1.2 删除元素](#12-删除元素)
  - [1.3 遍历其中的元素](#13-遍历其中的元素)
- [2. push_back和emplace_back的区别](#2-push_back和emplace_back的区别)
- [3. 在vector中查找元素](#3-在vector中查找元素)
  - [3.1 使用find函数](#31-使用find函数)


## 1 实现原理
C++中的vector本质上是一个动态数组（vector的底层实现原理为一维数组），它的元素是连续存储的，这意味着不仅可以通过迭代器访问元素，还可以使用指向元素的常规指针来对其进行访问。  
从实现原理来看：vector的随机访问是比较高效的, ( 可以直接使用数组下标的方式：vector[ i ] ）;

### 1.1 新增元素
插入vector中的新元素一般可以通过push_back()方法放入vector的末尾或者通过迭代器在vector的任何位置插入；  
对于迭代器的插入，首先是通过迭代器与第一个元素的距离知道要插入的位置，即int index = iter-vector.begin()，这个元素之后的所有元素都要向后移动一个位置，在空出来的位置上存入新增的元素

### *需要注意的点：*  
这里就会涉及到一个问题：既然vector是动态增长的，那么当其空间用完了的时候，基于vector连续存储的特性，要怎样处理空间不足的问题呢？  

STL中vector使用的策略便是:预先分配策略----这样可以有效的减少空间的分配次数  
vector会先分配一个比需求更大的空间作为预留， 

这里涉及到vector的几个属性：  
size():表示当前vector中已有的元素数量；  
capacity():表示vector的实际总容量，即预留空间大小  
reserver():表示要求vector的容量是多少，如果大于capacity则vector需要扩容  

举个例子：  
当vector的size大小为50时，这是capacity也为50的话，那么再新增一个元素的时候就需要对capacity进行扩容，变为100,而size加1即可；  
这里需要注意的是：  
只有在迫不得已的时候，才可以重新分配内存空间
vector扩容的时候为了保证其连续性，是去开辟新的更大的连续空间，再把vector中原来的元素移动到新空间，最后释放旧空间  
capcaity的具体扩容策略由标准库决定，（以等长方式扩容，以倍数（1.5或者2）方式扩容） *[ ps:具体的扩容方式和原因本人还没有理清楚，暂时不分析 ]*  

### 向vector中新增元素的几种方式
#### 在末尾添加元素
push_back()方法：
```c++
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> nums;
    
    for (int i=0; i<10; ++i) {
        nums.push_back(i);
    }
    
    for (auto &num: nums) {
        cout << num << " "; 
    }
    
    return 0;
}
```
emplace_back()方法：
```c++
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> nums;
    
    for (int i=0; i<10; ++i) {
        nums.emplace_back(i);
    }
    
    for (auto & num: nums) {
        cout << num << " "; 
    }
    
    return 0;
}
```
#### 在指定位置插入元素
insert方法：
```c++
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
    vector<int> nums(10);
    iota(nums.begin(),nums.begin()+10,1); //vector初始化;值为1~10
    for (auto& num:nums) {
        cout << num << " ";
    }
   cout << endl;
   
   cout <<"在头部插入4个元素0：" <<endl;
   nums.insert(nums.begin(),4,0); //在头部位置插入4个0
   for (auto &num : nums) {
    cout << num << " ";
   }
   cout << endl;
   
   cout <<"在中间插入元素100：" <<endl;
   nums.insert(nums.begin()+7,100);//在第8个位置开始插入100
   for (auto &num : nums) {
    cout << num << " ";
   }
   cout << endl;
   
   cout <<"在尾部插入元素11：" <<endl;
   nums.insert(nums.end(),11);//在尾部开始插入11
   for (auto &num : nums) {
    cout << num << " ";
   }
   
   return 0;
}
```
运行结果：
```c++
1 2 3 4 5 6 7 8 9 10 
在头部插入四个元素0：
0 0 0 0 1 2 3 4 5 6 7 8 9 10 
在中间插入元素100：
0 0 0 0 1 2 3 100 4 5 6 7 8 9 10 
在尾部插入元素11：
0 0 0 0 1 2 3 100 4 5 6 7 8 9 10 11 
```

### 1.2 删除元素

删除元素和新增差不多，可以使用pop_back删除最后一个元素或者通过迭代器删除任一元素；  
通过迭代器删除还是要先找到要删除元素的位置，即int index = iter - begin(), 而这个位置后面的每个元素都要向前移动一个元素的位置；  

举个例子：
```c++
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int main() {
    vector<int> nums(10);
    iota(nums.begin(),nums.end(),5); //nums初始化
    cout << "nums初始化:" << endl;
    for (auto num:nums) cout << num << " ";
    cout << endl;
    
    //使用pop_back方法：
    nums.pop_back();
    cout << "使用pop_back方法：" << endl;
    for (auto num:nums) cout << num << " ";
    cout << endl;
    
    //通过迭代器删除某个元素：
    vector<int>::iterator it = find(nums.begin(),nums.end(),9);
    cout << "删除数字9: " << endl;
    nums.erase(it);
    for (auto num:nums) cout << num << " ";
    cout << endl;
    
    return 0;
}
```
运行结果：  
![image](https://github.com/Feng3333/Cplusplus/blob/c9c514e5a8df8e6181ae8b0bcc82d4344ec5d4dd/images-folder/vector-delete1.png)

#### 清空vector  
清空vector中的所有元素：  
```
vector.clear();
```
需要注意的点：
clear时删除了vector中的所有元素，但vector的空间仍是存在的，即vector的size变成了0，但是capacity没有变化；  
由于vector是占用连续的内存空间的，所以如果vector占用的内存空间过大，并且使用了clear之后没有及时处理，会出现内存一直被占用的情况；  
处理方法：  
①每次clear之后重新初始化vector的capacity；  
②可以使用swap方法：  
```
vector<int> (nums).swap(nums);
```
③使用shrink_to_fit:  
使用shrink_to_fit可以减少容器的容量并且适应起大小，对于超出容器容量的部分会进行销毁
```c++
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
    vector<int> nums(100,11);
    for (auto num:nums) cout << num << "\t";
    cout << endl;
    
    nums.clear();
    cout << "清空nums中的元素后capacity的大小:" << nums.capacity() << endl;
    
    nums.resize(10);
    iota(nums.begin(),nums.begin()+10,1);
    for (auto num:nums) cout << num << "\t";
    cout << endl;
    cout << "重新放入10个元素后capacity的大小:" << nums.capacity() << endl;
    
    nums.shrink_to_fit();
    cout << "使用shrink_to_fit后capacity的大小:" << nums.capacity() << endl;
    
    return 0;
}
```

### 1.3 遍历其中的元素
#### 1.3.1 下标索引遍历 
```c++
#include <iostream>
#include <vector>

using namespace std;
int main() {
    vector<int> nums(10);
    for (int i=0; i<nums.size(); i++) {
        cout << nums[i] << "\n";
    }
    return 0;
}
```
#### 1.3.2 通过迭代器遍历
```c++
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;
int main() {

    vector<int> nums(10);
    iota(nums.begin(),nums.end(),1);
    for (vector<int>::iterator it = nums.begin() ; it != nums.end(); it++) {
         cout << *it << endl;
    }
    
    return 0;
}
```
#### 1.3.3 使用auto关键字
```c++
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;
int main() {

    vector<int> nums(10);
    iota(nums.begin(),nums.end(),100);
    for (auto& num: nums) {
        cout << num << endl; 
    }
    
    return 0;
}
```

## 2 push_back和emplace_back的区别

### 一句话概括：  
push_back()是构造了新元素之后，拷贝或移动到容器的末尾；而emplace_back()则是直接构造了新元素并将它添加到容器的末尾。  

### 底层实现机制不同：
push_back(): 向容器尾部添加元素时，首先会创建这个元素，然后再将这个元素拷贝或者移动到容器中（如果时拷贝的话，事后会自动销毁先前创建的这个元素）；  
emplace_back(): 直接在容器尾部创建这个元素，省去了拷贝或移动元素的过程；  

演示代码：
```c++
#include <iostream>
#include <vector>

using namespace std;

class testDemo {
private:
    int num;
    
public:
    testDemo(int num):num(num) {
        cout << "调用构造函数" << endl;
    }
    
    testDemo(const testDemo& other) :num(other.num) {
        cout << "调用拷贝构造函数" << endl;
    }
    
    testDemo(testDemo&& other) :num(other.num) {
        cout << "调用移动构造函数" << endl;
    }
};

int main(){
    cout << "emplace_back: " << endl;
    vector<testDemo> nums1;
    nums1.emplace_back(2);
    for (auto num:nums1) cout << num ;
    
    cout << "push_back: " << endl;
    vector<testDemo> nums2;
    nums2.push_back(22);
    for (auto num:nums2) cout << num ;
    
    return 0;
}
```
运行结果：
```
emplace_back: 
调用构造函数
push_back: 
调用构造函数
调用拷贝构造函数
```
## 3 在vector中查找元素
### 3.1 使用find函数
 不同于其他容器, vector中使用find并没有提供 .find()的方法，需要手动补全:
 ```cpp
 ...
 vector<int> nums = {1,2,3,4,5,7};
 vector<int>::iterator it = find(nums.begin(), nums.end(), 6);
 ...
 ```
