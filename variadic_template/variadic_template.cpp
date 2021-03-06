/*
@seanlau

*/
#include "variadic_template.h"

template<typename T>
struct DoSome
{
	void operator()(T t){ cout << t << endl; }
};
DoSome<int> dosome;

int main()
{
	expand([](int i) {cout << i << endl; }, 1, 2, 3);//c++11 lambda expression
	expand([](auto i) {cout << i << endl; }, 1, 2.6, 3);//c++14 lambda expression
	expand3(dosome, 1, 2, 3);//仿函数（函数指针也类似） 只能一种类型
	
	//逗号表达式展开参数
	expand2(10, 3.6);
	expand1(1.6, 2.2, 3);//右值引用 完美转发
	return 0;

}
