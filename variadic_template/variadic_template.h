#ifndef VARIADIC_TEMPLATE_H
#define VARIADIC_TEMPLATE_H
#include <iostream>
#include <vector>
#include <initializer_list>
using namespace std;

template<typename T>
void printarg(T t)
{
	cout << t << endl;
}


template<typename... Args>
void expand2(Args... args)
{
	//int a[] = { (printarg(args), 0)... };
	std::initializer_list<int>{(printarg(args), 0)...};
}

template<class... Args>
void expand1(Args&&...args)//右值引用 (可以传入左值和右值)
{
	//std::forward 按数据原本的左右属性进行完美转发
	//int a[] = { (printarg(std::forward<Args>(args)), 0)... };
	initializer_list<int>{(printarg(std::forward<Args>(args)), 0)...};
}


template<class F, class... Args>
void expand(const F& f, Args&&...args)
{
	initializer_list<int>{(f(std::forward<Args>(args)), 0)...};
}

template<class F, class... Args>
void expand3(F& f, Args&&...args)
{
	initializer_list<int>{(f(std::forward<Args>(args)), 0)...};
}


#endif //VARIADIC_TEMPLATE_H
