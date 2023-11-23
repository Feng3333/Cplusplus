# enum: C++枚举类型

## 枚举类型
1. enum class : 枚举值位于enum的局部作用域内，枚举值不会隐式的转换成其他类型;
2. 普通的enum : 枚举值名字与枚举类型本身位于同一个作用域内, 枚举值隐式的转换成整数;
 
## 作用域
#### 1. 枚举类
枚举名字前面带class修饰符的枚举就是枚举类, 他的成员作用域在enum内部, 外部是不能直接访问的, 需要通过类名::访问;
简单举例 :  
```cpp
enum class Lights {
  red,
  green,
  yellow
};
```
访问方式 : 在类外面直接访问red或者其他元素都是不行的, 需和类访问公有成员一样的方式访问 :  
```cpp
Lights::red
```

#### 2. 普通枚举
普通枚举就是不带class修饰符的枚举, 他的枚举值名字和枚举类型本身位于同一个作用域中, 不需要通过名字访问;
简单举例 :
```cpp
enum Nums {
  zero,
  one,
  two
};
```
访问方式 : 直接使用枚举值名字即可:  
```cpp
one;
Nums::one;
```

#### 3. 未命名枚举
   还有一种枚举既不加class修饰, 也不写枚举名字, 而是只有枚举值:
```cpp
enum {
  red,
  green,
  yellow
};
```

访问方式:
```cpp
red;
```

## 隐式类型转换
枚举值常用一些整数类型表示, 每个枚举值一般是一个整数;  
如果不显示指定枚举值, 那么枚举值默认是从 0 开始的正整数, 这一点对于所有枚举类型都是一样的.  
```cpp
enum class Lights {
  red,   // 0
  green, // 1
  yellow // 2
};

enum Nums {
  zero, // 0
  one,  // 1
  two   // 2
};

enum {
  red,   // 0
  green, // 1
  yellow // 2
};
```

#### 不同点
使用class修饰的枚举类型, 它的枚举值时不能隐式转换成其他类型的, 比如:  
```cpp
int color = Lights::red;   // 错误, 枚举类不能隐式转换
int num = Nums::one;       // 正确，普通枚举包括未命名枚举可以隐式转换
```

## 显示指定枚举值类型
```cpp
enum class Nums:int {
  zero, // 0
  one,  // 1
  two   // 2
}; // sizeof(Nums) == 4

enum class Lights:char {
  red,   // 0
  green, // 1
  yellow // 2
}; // sizeof(Nums) == 1
```

## 指定枚举值中元素的值
```cpp
enum class Nums:int {
  zero = 0,
  one = 1,
  two = 2
}; 
```

如果只给第一个枚举元素指定了值，那么剩下的枚举元素的值会依次累加:  
```cpp
enum class Nums:int {
  three = 3,
  four,  // 4
  five,  // 5
}; 
```
