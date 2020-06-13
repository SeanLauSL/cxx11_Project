#pragma once
#include <iostream>
using namespace std;

class Human {
public:
	Human(int age = 5)
		: mage(age)
	{
		this->name = new char[10];
		cout << "Constructor with age��" << endl;
	}

	Human(int age, const char* name) {
		mage = age;
		//this->name = name;
		this->name = new char[10];
		strcpy(this->name, name);
		cout << "Constructor with parameters��" << endl;
	}

	Human(const Human& human)
		:mage(human.mage)
	{
		//this->name = human.name;
		this->name = new char[10];
		strcpy(this->name, human.name);
		cout << "copy Constructor��" << endl;
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
		/*�˴�û�����¿����ڴ濽�����ݣ���human����Դֱ�Ӹ���ǰ�����ٰ�human�ÿ�*/
		this->name = human.name;
		human.name = nullptr;
		cout << "move Constructor��" << endl;
	}

	Human& operator=(Human&& src)
	{
		cout << "operator= (Human&&)" << endl;
		if (this == &src)
			return *this;

		delete name;

		mage = src.mage;
		/*�˴�û�����¿����ڴ濽�����ݣ���src����Դֱ�Ӹ���ǰ�����ٰ�src�ÿ�*/
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
		cout << "age��" << mage << " name��" << name << endl;
	}

	int getAge()
	{
		return mage;
	}

private:
	int mage;
	char* name;
};