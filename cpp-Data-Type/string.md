# string类
头文件：  
```c++
#include <string>
```
string本质上可以看做是一种vector< char >,也就是元素为char的vector。所以有关 vector的基本操作都可以在string上进行。  

## string的基本操作

### string的创建和初始化
```c++
#include <iostream>
#include <string>

using namespace std;

int main(){
  string s1 = "abc";
  string s2(3,'a');
  string s3("efg");
  string s4(s3);
  string s5;
  s5 = s1;
  string s6(s1.begin(),s1.end());
  string s7(s3,0,2); //截取的字符串，位置pos，长度len
  
  cout << "s1:" << s1 << endl;
  cout << "s2:" << s2 << endl;
  cout << "s3:" << s3 << endl;
  cout << "s4:" << s4 << endl;
  cout << "s5:" << s5 << endl;
  cout << "s6:" << s6 << endl;
  cout << "s7:" << s7 << endl;
  return 0;
}
```

### string的拼接和附加
```c++
#include <iostream>
#include <string>

using namespace std;

int main() {
  string s1 = "hello ";
  string s2 = "world";
  string s3;
  //字符串拼接
  s3 = s1+s2;
  cout << "s3: " << s3 << endl; 
  s1 += s2;
  cout << "s1: " << s1 << endl; 
  
  //使用append()的方式在末尾添加字符
  s3.append('z');
  cout << "s3: " << s3 << endl;
  
  //使用push的方法在字符串末尾添加字符
  for (int i=0; i<3; ++i) {
    s2.push_back('!');
  }
  cout << "s2: " << s2 << endl;
  
  //使用pop的方法在字符串末尾删除字符
  s2.pop_back();
  cout << "s2: " << s2 << endl;
  return 0;
}
```

### string类的遍历和访问
```c++
#include <iostream>
#include <string>

using namespace std;

int main() {
  string s1("Hello World!");
  int len = s1.size(); //这里也可以说用length()
  
  for (int i=0; i<len; i++) {
    cout << s1[i] << endl; //s1[i]的类型为char类型而非string类
  }
  
  return 0;
}
```
也可以使用at对string进行访问，但是at会有越界检查,如果index越界，程序会异常跳出执行(无论是在Debug还是在Release编译的环境下)
```c++
#include <iostream>
#include <string>

using namespace std;

int main() {
  string s1("Hello World!");
  
  for (int i=0; i<len; i++) {
    cout << s1.at(i) << endl; 
  }
  
  return 0;
}
```

### string中的插入和删除

插入 insert()：
```c++
#include <iostream>
#include <string>

using namespace std;

int main() {
  string str("aaa");
  cout << "str is empty? " << str.empty() << endl;
  
  str.clear(); //清空，常用于再次初始化一个字符串
  cout << "str is empty? " << str.empty() << endl;
  
  str = "abcdefghijk";
  //insert()在指定位置插入字符串,具体的重载可以在 basic_string.h中查看
  str.insert(1,1,'A'); //insert(size_type index, size_type count, char ch)
  cout << "str insert A int index 1: "  << str << endl;
  
  return 0； 
}
```
删除 erase():   
erase的三种用法：  
```c++
//函数原型
string& erase(size_t pos = 0, size_t n = npos);
iterator erase(iterator position);
iterator erase(iterator first, iterator last);

//具体用法：
erase(pos, n);      //删除从pos开始的n个字符,比如erase(0, 1)就是删除第一个字符
erase(position);    //删除从position处的一个字符(position是个string类型的迭代器)
erase(first, last); //删除从first到last之间的字符(first和last都是迭代器)
```

```c++
#include <iostream>
#include <string>

using namespace std;

int main() {
  string str("abcdefghijk");
  cout << "str : " << str << endl;
  str.erase(0,1); //删除从0开始的后面的1个字符
  cout << "str is delete: " << str << endl;
  
  return 0;
}
```
## substr在string类的运用
函数原型: string substr(size_t pos = 0, size_t len = npos) const;  
功能: 从子字符串中获取想要的子字符串  
参数：
pos: 想要获取的子字符串的第一个字符的位置，如果pos等于字符串长度，则该函数返回一个空字符串,如果该长度大于字符串长度，则抛出一个out_of_range。 注：第一个字符的下标从0开始。  
len： 子字符串中要包含的字符数, string::npos的值表示知道字符串末尾的所有字符。    
返回值：带有对象子字符串的字符串对象。
```c++
#include <iostream>
#include <string>

using namespace std;

int main() {
  string str("abcdefg");
  cout << "str : " << str << endl;
  string res = str.substr(0,3);
  cout << "res : " << res << endl;
  
  return 0;
}
```

## find在string类中的运用
find()的普通用法:  

使用find可以在string中查找对应的字符串或者字符,如果所查找的目标字符或者目标字符串不在当前字符串中出现，则会返回string::npos;  
注意点：find会查找到(从起始位置开始)当前字符串中第一次出现目标字符串的位置，之后便会返回;  
string::npos是一个静态成员常量，表示size_t的最大值（Maximum value for size_t）。该值表示“直到字符串结尾”，通常被用作返回值表明没有匹配到相应的字符。
```c++
#include <iostream>
#include <string>

using namespace std;

int main() {
  string s1("abcdefghijk");
  string s2("efg");
  auto pos1 = s1.find(s2);
  if (pos1 == string::npos) {
    cout << "not found !" << endl;
  }
  else {
    cout << "position: " << pos1 << endl;
  }
  
  char ch = 'a';
  auto pos2 = s1.find(ch);
  if (pos2 != string::npos) {
    cout << "position: " << pos2 << endl;
  }
  else {
    cout << "not found !" << endl;
  }
}
```
