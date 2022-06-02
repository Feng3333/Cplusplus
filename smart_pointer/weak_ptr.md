# weak_ptr

## weak_ptr简介
weak_ptr 被设计为与 shared_ptr 共同工作，可以从一个 shared_ptr 或者另一个 weak_ptr 对象构造而来。  
weak_ptr 是为了配合 shared_ptr 而引入的一种智能指针，它更像是 shared_ptr 的一个助手而不是智能指针，因为它不具有普通指针的行为，没有重载 operator* 和 operator->;  
表明其是功能较弱的智能指针。它的最大作用在于协助 shared_ptr 工作，可获得资源的观测权，像旁观者那样观测资源的使用情况;  
观察者意味着 weak_ptr 只对 shared_ptr 进行引用，而不改变其引用计数，当被观察的 shared_ptr 失效后，相应的 weak_ptr 也相应失效  


## weak_ptr基本用法
```c++

```
