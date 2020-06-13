#pragma once
#include <iostream>
using namespace std;

class Human {
public:
	Human(int age = 5)
		: mage(age)
	{
		this->name = new char[10];
		cout << "Constructor with age！" << endl;
	}

	Human(int age, const char* name) {
		mage = age;
		//this->name = name;
		this->name = new char[10];
		strcpy(this->name, name);
		cout << "Constructor with parameters！" << endl;
	}

	Human(const Human& human)
		:mage(human.mage)
	{
		//this->name = human.name;
		this->name = new char[10];
		strcpy(this->name, human.name);
		cout << "copy Constructor！" << endl;
	}

	Human& operator=(const Human &src)
	{
		cout << "operator=" << endl;
		if (this == &src)
			return *this;

		delete[]name;

		mage = src.mage;
		name = new char[10];
		strcpy(this->name, src.name);

		return *this;
	}

	Human(Human&& human)
		:mage(human.mage)
	{
		/*此处没有重新开辟内存拷贝数据，把human的资源直接给当前对象，再把human置空*/
		this->name = human.name;
		human.name = nullptr;
		cout << "move Constructor！" << endl;
	}

	Human& operator=(Human&& src)
	{
		cout << "operator= (Human&&)" << endl;
		if (this == &src)
			return *this;

		delete name;

		mage = src.mage;
		/*此处没有重新开辟内存拷贝数据，把src的资源直接给当前对象，再把src置空*/
		name = src.name;
		src.name = nullptr;

		return *this;
	}

	~Human()
	{
		cout << "~Stack()" << endl;
		delete[] name;
		name = nullptr;
	};

	void print(void) {
		cout << "age：" << mage << " name：" << name << endl;
	}

	int getAge()
	{
		return mage;
	}

private:
	int mage;
	char* name;
};