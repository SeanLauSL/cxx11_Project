#ifndef __TEST_MODULE_THREAD_H
#define __TEST_MODULE_THREAD_H

#include "moduleThreadcxx11.h"
#include "debugMsg.h"

template<typename T>
class TestModuleThread :
	public ModuleThread
{
public:

	TestModuleThread(T &_data) : ModuleThread()
	{
		data = _data;
	}

	virtual ~TestModuleThread() {}

public:
	void callBackFuc()
	{
		//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s %d\n", "thread ", getTid().c_str(), "Test:", this->data);
		MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "Test:", this->data);
	}


private:
	T data = NULL;
};

//显式实例化,可以把成员函数实现放到cpp，否则必须全部为内联
//template class TestModuleThread<int>;

#endif //__MODULE_THREAD_CXX11_H