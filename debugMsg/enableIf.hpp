#ifndef __ENABLE_IF_H
#define __ENABLE_IF_H
#include <iostream>
#include <initializer_list>
#include <stdio.h>

//test for std::enable_if
class Debug
{
public:
	//template <bool flag, typename ...Args>
	//void SDK_PRINT();
	
	///////////////////////////////////////////////////////////////////////////////////////////
	//test for std::enable_if

	//定义不同的系统的特化版本  
	template<int flag, typename T>
	static void test(T arg, typename std::enable_if<flag == 0, typename T>::type* = 0)//0
	{
		std::cout << "0: " << arg << std::endl;
	}


	template<int flag, typename T>
	static void test(T arg, typename std::enable_if<flag == 1, typename T>::type* = 0)//1
	{
		std::cout << "1: " << arg << std::endl;
	}


	template<int flag, typename T>
	static void test(typename std::enable_if<flag == 2, typename T>::type arg)//2
	{
		std::cout << "2: " << arg << std::endl;
	}

	template<int flag, typename T>
	static void test(typename std::enable_if<flag == 3, T>::type arg)//3,与2一样
	{
		std::cout << "3: " << arg << std::endl;
	}

	template<int flag, typename T>
	static void test(T arg, typename std::enable_if<flag == 4, void>::type* = 0)//4 与0、1一样
	{
		std::cout << "4: " << arg << std::endl;
	}


	template<typename F, typename... Args>
	static void expand(const F& f, Args&&...args)
	{
		std::initializer_list<int>{(f(std::forward<Args>(args)), 0)...};
	}

	template<int flag, typename ...Args>
	static void test(Args ...args, typename std::enable_if<flag == 5, Args>::type ..._a)//5 error 暂时无法不声明类型去使用可变参数
	{
		expand([](auto i) {std::cout << i << " "; }, args...);
	}

	template<int flag, typename ...Args>
	static void test(typename std::enable_if<flag == 6, Args>::type&& ...args)//6
	{
		std::cout << __LINE__ << ": ";
		expand([](auto i) {std::cout << i << " "; }, args...);
		std::cout << std::endl;

	}
	///////////////////////////////////////////////////////////////////////////////////////////
public:

	enum MSG_LEVEL
	{
		INFO = 1,
		ALWAYS = 45
	};
	

};

#endif //__ENABLE_IF_H
