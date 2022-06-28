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

### string类的其他操作
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

## find在string类中的运用
find()的普通用法:  

使用find可以在string中查找对应的字符串或者字符,如果所查找的目标字符或者目标字符串不在当前字符串中出现，则会返回string::npos:  
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
