# C++20 Ranges
C++20引入了范围(Ranges)的新特性，这是一种现代化的、功能强大的处理序列数据的机制。范围(Ranges)的目标是提供一种更简洁、更易读、更安全且更高效的方式来操作数据序列，代替传统的迭代器和手动循环操作。

- 范围概念： 范围(Ranges)是一种统一的序列表示形式，它可以是一个数组、容器、字符串、文件流等一切能够返回一系列元素的数据源。范围将序列数据和处理数据的算法解耦，使得我们可以在一个范围上链式地应用多个算法，实现流畅且功能强大的数据处理操作。
  
- 范围视图： 范围视图(Ranges Views)是范围的一种扩展，它允许我们在一个范围上进行类似于管道操作的连续转换和筛选操作。范围视图是一种惰性求值的机制，它们并不直接生成数据，而是在需要时根据需要生成数据。
  
- 范围算法： C++20引入了一组新的范围算法，这些算法使用统一的范围接口而不是传统的迭代器。这些算法可以直接作用在范围上，而不需要指定范围的开始和结束迭代器。范围算法提供了丰富的功能，包括排序、查找、变换、筛选、分组等，它们与范围视图一起可以完成大部分序列处理任务。
  
- 范围适配器： 范围适配器(Ranges Adapters)是一种范围视图，用于将范围视图转换为不同类型的范围视图，或者在范围上应用其他操作。范围适配器类似于算法，但它们返回的是一个新的范围视图，而不是处理范围上的元素。
  
- 标准范围库： C++20引入了<ranges>头文件，其中包含了范围概念、范围视图、范围算法和范围适配器的相关类和函数。通过包含这个头文件，可以在项目中使用C++20 Ranges特性。

## 头文件
```cpp
#include <ranges>
```

## 使用方法&代码示例

### 1. 排序
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>
 
int main() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
 
    // 使用范围视图和范围算法对序列进行排序
    std::ranges::sort(numbers);
 
    for (const int num : numbers) {
        std::cout << num << ' ';
    }
 
    return 0;
}
```

### 2. 查找
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>
 
int main() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
 
    // 使用范围视图和范围算法查找序列中的特定值
    auto it = std::ranges::find(numbers, 5);
    if (it != numbers.end()) {
        std::cout << "Found at index: " << std::distance(numbers.begin(), it) << '\n';
    } else {
        std::cout << "Not found\n";
    }
 
    return 0;
}
```

### 3. 变换
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>
 
int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
 
    // 使用范围视图和范围算法将每个元素平方
    auto squared_numbers = numbers | std::views::transform([](int num) { return num * num; });
 
    for (const int num : squared_numbers) {
        std::cout << num << ' ';
    }
 
    return 0;
}
```

### 4. 筛选
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>
 
int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
 
    // 使用范围视图和范围算法筛选出所有偶数
    auto even_numbers = numbers | std::views::filter([](int num) { return num % 2 == 0; });
 
    for (const int num : even_numbers) {
        std::cout << num << ' ';
    }
 
    return 0;
}
```

### 5. 分组
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>
 
int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 
    // 使用范围视图和范围算法将序列分组成奇数和偶数
    auto odd_and_even = numbers | std::views::group_by([](int a, int b) { return (a % 2) == (b % 2); });
 
    for (auto it = odd_and_even.begin(); it != odd_and_even.end(); ++it) {
        std::cout << "Group: ";
        for (const int num : *it) {
            std::cout << num << ' ';
        }
        std::cout << '\n';
    }
 
    return 0;
}
```

### 6. 范围适配器
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>
 
int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
 
    // 使用范围视图和范围适配器对序列进行变换和筛选操作
    auto squared_and_even = numbers | std::views::transform([](int num) { return num * num; })
                                    | std::views::filter([](int num) { return num % 2 == 0; });
 
    for (const int num : squared_and_even) {
        std::cout << num << ' ';
    }
 
    return 0;
}
```
