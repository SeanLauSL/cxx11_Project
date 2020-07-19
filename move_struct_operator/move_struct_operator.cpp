#include <iostream>
using namespace std;

#include "human.hpp"

Human GetHuman(Human &s)
{
	//return s;

	Human tmp(s.getAge());//构造tmp
	return tmp;//tmp 赋值给临时对象，调用移动构造函数

}
int main()
{
	//////////////////////////////////////////////////////////////////////////
	//移动构造和移动赋值测试
	//copy class
	Human s0(12, "woman"); //Constructor
	Human s1 = s0;//copy Constructor
	Human s11;
	s11 = s1;//copy operator=

	//move class (std::move)(std::move用在含动态资源的类对象上，如果用在静态资源上，如 char[10],仍然会复制，因为没有对应的移动构造函数)
	Human s2(std::move(s1));//move Constructor  //左值转右值，防止复制操作，但是s1的动态资源已经被清空，无法继续访问，如name。
	//Human s2 = std::move(s1);//与上式等效 
	Human s3;
	s3 = std::move(s2);//move operator=

	//move class (移动构造和移动赋值)
	Human s4 = GetHuman(s0);//move Constructor
	Human s5;
	s5 = GetHuman(s0);//move operator=  获取右值时，调用移动赋值，防止复制操作

	//[总结]：声明同时初始化的都是调用构造函数；声明完成后再赋值的调用赋值运算符；
	//参数为左值则调用常规的构造或赋值；参数为右值则调用移动构造或移动赋值；

	return 0;
}