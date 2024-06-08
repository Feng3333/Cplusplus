# C++入栈顺序
c或c++中的都是从右往左入栈的，在调用函数时，若参数需要运算则先运算，然后入栈，在调用函数

## 目录
 - [1. 入栈顺序](#1-入栈顺序)
 - [2. 函数的执行顺序](#2-函数的执行顺序)

## 1. 入栈顺序
- 栈区是从高地址向低地址扩展，是一块连续的内存区域，遵循先进后出，后进先出(FILO)原则;
- 程序中栈顶在低位，栈底在高位  
程序的入栈顺序演示：

```c++
#include <iostream>

void push(int a, int b, int c) 
{
    std::cout << "a = " << a << ", ptr = " << &a << std::endl;
    std::cout << "b = " << b << ", ptr = " << &b << std::endl;
    std::cout << "c = " << c << ", ptr = " << &c << std::endl;
}

int main() {
    int a = 0, b = 1, c = 2;
    push(a, b, c);
    return 0;
}
```

在windows上的输出结果 :
```
a = 0, ptr = 0x61fdf0
b = 1, ptr = 0x61fdf8
c = 2, ptr = 0x61fe00
```
在linux (x86_64) 上的输出结果 :
```
a = 0, ptr = 0x7fffcba586bc
b = 1, ptr = 0x7fffcba586b8
c = 2, ptr = 0x7fffcba586b4
```
解释说明：
先入栈的在栈底，应该存储在高位，后入栈的在栈底，应该存储在低位； 所以参数c最先入栈，参数a最后入栈；函数调用是从右往左入栈的；  
- 需要注意的点：  
在linux的64位机器中，会先用寄存器传参，当参数少于7个时， 参数从左到右放入寄存器: rdi, rsi, rdx, rcx, r8, r9。当参数为7个以上时， 前 6 个与前面一样， 但后面的依次从 “右向左” 放入栈中；  
此外，当linux 上开启地址随机化 即：/proc/sys/kernel/randomize_va_space 值不是0 ，那么每次虚拟地址都是不同的。如果 这个值是0，则不会随机化地址，这种情况下运行每次地址都会一样。  

继续在push中增加入参时：  
```
a = 1, ptr = 0x7fffffffddfc
b = 2, ptr = 0x7fffffffddf8
c = 3, ptr = 0x7fffffffddf4
d = 4, ptr = 0x7fffffffddf0
e = 5, ptr = 0x7fffffffddec
f = 6, ptr = 0x7fffffffdde8
g = 7, ptr = 0x7fffffffde10
h = 8, ptr = 0x7fffffffde18
i = 9, ptr = 0x7fffffffde20
```
```
a = 1, ptr = 0x7fffffffddcc
b = 2, ptr = 0x7fffffffddc8
c = 3, ptr = 0x7fffffffddc4
d = 4, ptr = 0x7fffffffddc0
e = 5, ptr = 0x7fffffffddbc
f = 6, ptr = 0x7fffffffddb8
g = 7, ptr = 0x7fffffffdde0
h = 8, ptr = 0x7fffffffdde8
i = 9, ptr = 0x7fffffffddf0
j = 10, ptr = 0x7fffffffddf8
k = 11, ptr = 0x7fffffffde00
o = 12, ptr = 0x7fffffffde08
p = 13, ptr = 0x7fffffffde10
q = 14, ptr = 0x7fffffffde18
r = 15, ptr = 0x7fffffffde20
```

## 2. 函数的执行顺序
- 1. 函数参数有运算，则先计算完参数中的全部运算在执行函数;
- 2. 函数参数中的运算需要区分时调用前执行还是调用后执行的函数 (i++, ++i);
- 3. 入栈的是临时变量还是实际变量；

示例代码：
```c++
#include <iostream>

int main() {
    int i = 1;
    
    /* 先计算，两次i++后i的值为3，传递的是实际变量i的值 */
    printf("%d, %d\n", ++i, ++i);

    /* 
      i++入栈的i, 然后执行i++，所以需要临时变量将i的值保存下来在计算i++; ++i先计算再入栈；
      结果为 5, 3；
    */
    printf("%d, %d\n", ++i, i++);

    /* 
      后一个i++保存临时变量 i2 = 5，然后i自增 = 6；
      前一个i++先保存临时变量 i1 = 6，然后i自增 = 7；
      右边的参数先入栈push i2
      左边的参数后入栈push i1
    */
    printf("%d, %d\n", i++, i++);

    /*
      后一个++i，先自增 i = 8；
      前一个i++，先保存临时变量i1 = 8， 然后i自增 = 9；
      右边参数先入栈push i = 9；
      左边参数后入栈push i1 = 8；
    */
    printf("%d, %d\n", i++, ++i);

    return 0;
}
```
具体信息也可从编译后的汇编代码查看；

- 深入思考~~   
入栈顺序：参数从右往左入栈；  
计算顺序：先入栈的先计算，计算完后再入栈；  
传递的参数：实际变量，还是临时变量；  
```c++
#include <iostream>

void push(int a, int b, int c) {
    printf("%d, %d, %d\n", a, b, c);
}

int main() {
    int i = 0;
    push(i++, i++, i++);

    i = 0;
    push(++i, ++i, ++i);

    i = 0;
    push(++i, i++, ++i);

    i = 0;
    push(i++, ++i, ++i);

    return 0;
}
```
```
2, 1, 0
3, 3, 3
3, 1, 3
2, 3, 3
```
