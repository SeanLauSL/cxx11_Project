# cxx11_Project
C++11特性的Visual studio 2017工程，如可变参数模板队列、移动语义、控制调试信息、创建模块线程

* moduleThreadcxx11

```text
1. 使用c++11特性建立一个跨平台的线程抽象基类：模块线程的创建、启动、使用RAII特性的线程挂起和恢复、以及RAII特性的读写锁。
2. 其派生类只需重写工作任务的回调函数，也就是抽象基类的一个纯虚函数callBackFunc()，即可快速创建模块线程。
3. 同时基于模板元、可变参数模板创建了一个可分级控制的调试信息输出类Msg。
```
* deque_template_thread_safe

```text
线程安全的deque模板的FIFO结构。
```

* variadic_template

```text
可变参数模板参数展开。
```

* move_struct_operator

```text
c++11移动语义测试。
```
