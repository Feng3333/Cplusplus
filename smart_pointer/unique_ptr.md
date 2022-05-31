# unique_ptr的基本介绍
unique_ptr是定义在头文件 \<memory\> 中的智能指针。  
```c++
#include <memory>
```

## unique_ptr的基本规则
①：持有对对象的独有权---->即两个unique_ptr不能指向同一个对象，unique_ptr不共享他所管理的对象;  
②：无法复制到其他unique_ptr，无法通过值传递到函数，也无法用于需要副本的任何标准模板库算法；  
③：只能移动 unique_ptr，即对资源管理权限可以实现转移。----->这意味着，内存资源所有权可以转移到另一个 unique_ptr，并且原始 unique_ptr 不再拥有此资源;  
