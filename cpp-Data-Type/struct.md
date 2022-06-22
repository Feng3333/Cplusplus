# struct
## 基本概念
struct (结构体) 是由一系列具有相同类型或不同类型的数据构成的数据集合，也叫结构。  
struct 可以将不同类型的数据存放在一起，作为一个整体进行处理。  

结构体的大小遵循 *内存对齐* 规则

## struct的定义和使用

语法：struct 结构体名 {结构体成员列表}；
举个例子：  
```c++
#include <iostream>
#include <string> 

struct person {
  std::string name;
  int age;
} p1;

int main() {
  p1.name = "Messi";
  p1.age = 35;
  std::cout << "p1 : " << p1.name << " " << p1.age << std::endl;
  
  struct person p2 = {"Neymar",30};
  std::cout << "p2 : " << p2.name << " " << p2.age << std::endl;
  return 0;
} 
```
 
### struct可以放入数组中
```c++
#include <iostream>
#include <string> 

struct person {
  std::string name;
  int age;
};

int main() {
  person p[2] = {
    {"Messi",35},
    {"Neymar",30}
  };
  
  for (int i=0; i<2; ++i) {
    std::cout << "name : " << p[i].name << " age : " << p[i].age << std::endl; 
  }
  
  return 0;
}
```

### struct中指针的运用
.用来访问结构体中的成员/属性  
->用来访问其指向的结构体成员/属性  

```c++
#include <iostream>
#include <string> 

struct person {
  std::string name;
  int age;
};

int main() {
  person p1 = {"Messi",35};
  std::cout << "p1 : " << p1.name << " " << p1.age << std::endl;
  
  person *point = &p1;
  std::cout << "name: " << (*point).name << " age: " << (point).age << std::endl;
  std::cout << "name: " << point->name << " age: " << point->age << std::endl;
  
  return 0;
}
```

### struct中可嵌套使用struct
```c++
#include <iostream>
#include <string>

struct person {
  std::string name;
  int age;
};


struct team {
  std::string id;
  struct person player;
};

int main() {
  team t1;
  t1.id = "FCB";
  t1.player.name = "Messi";
  t1.player.age = 35;
  
  std::cout << "t1 name: " << t1.id << "  t1 player:" << t1.player.name << " " << t1.player.age << std::endl;
  
  return 0;
}
```

### struct可作为函数的参数
传递方式有：值传递，地址传递  
```c++
#include <iostream>
#include <string>

struct person {
  std::string name;
  int age;
};

//值传递
void printPerson1(person per) {
  per.age = 30;
  std::cout << "person :" << per.name << " " << per.age << std::endl;
};

//地址传递
void printPerson2(person *per) {
  per->age = 35;
  std::cout << "person :" << per->name << " " << per->age << std::endl;
};

int main() {
  struct person per = {"Messi",13};
  std::cout << "person :" << per.name << " " << per.age << std::endl;
  
  printPerson1(per);
  printPerson2(&per);
  
  return 0;
}

```
