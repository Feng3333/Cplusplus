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
