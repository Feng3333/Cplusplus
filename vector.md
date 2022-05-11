# vector的定义和原理和与vector的相关操作







## push_back() 和 emplace_back() 的区别

一句话概括：  
push_back()是构造了新元素之后，拷贝到容器的末尾；而emplace_back()则是直接构造了新元素并将它添加到容器的末尾，省略了拷贝构造这一操作。  

