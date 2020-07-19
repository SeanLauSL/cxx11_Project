#ifndef __TEST_MODULE_THREAD_H
#define __TEST_MODULE_THREAD_H

#include "moduleThreadcxx11.h"

template<typename T>
class TestModuleThread :
	public ModuleThread
{
public:

	TestModuleThread(T &_data) :ModuleThread()
	{
		data = _data;
	}

	virtual ~TestModuleThread() {}

public:
	void callBackFuc()
	{
		std::cout << "Test: " << this->data << std::endl; std::cout.flush();
	}


private:
	T data;
};

//��ʽʵ����,���԰ѳ�Ա����ʵ�ַŵ�cpp���������ȫ��Ϊ����
//template class TestModuleThread<int>;

#endif //__MODULE_THREAD_CXX11_H