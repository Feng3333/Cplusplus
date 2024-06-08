# memset
memset 是 C 标准库中的一个函数，用于将一块内存区域设置为指定的值。在 C++ 中也可以使用这个函数，因为 C++ 是 C 的超集。函数的原型在头文件 <cstring> 中：
```cpp
void *memset(void *ptr, int value, size_t num);
```
- ptr 是指向要填充的内存块的指针。
- value 是要设置的值，按字节设置。
- num 是要设置的字节数。

代码示例:  
```cpp
#include <cstring>
#include <iostream>

int main() {
    int memo[100];
    memset(memo, -1, sizeof(memo));
    
    for (int i = 0; i < 10; ++i) {
        std::cout << memo[i] << " ";
    }
    
    return 0;
}
```
memset(memo, -1, sizeof(memo)) 会将 memo 数组的每个字节都设置为 0xFF。在大多数系统上，memo中每个 int 元素将会变为 -1。
