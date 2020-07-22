#ifndef __TEST_MODULE_THREAD_H
#define __TEST_MODULE_THREAD_H

#include "moduleThreadcxx11.h"
#include "debugMsg.h"

template<typename T>
class TestModuleThread :
	public ModuleThread
{
public:

	TestModuleThread(const T &_data) : ModuleThread()
	{
		data = _data;
	}

	virtual ~TestModuleThread() {}

	// disable copy constructors
	TestModuleThread(const TestModuleThread&) = delete;
	TestModuleThread& operator=(const TestModuleThread&) = delete;

private:
	//override for the thread body
	void callBackFuc() override
	{
		WGuard wLock(this->rwLock);//write lock
		//RGuard rLock(this->rwLock);//read lock
		MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "Test:", this->data++);
	}

	//override for restarting the thread after stop() 
	//create an object as same as that before stop()
	ModuleThread* restoreObject() override
	{
		return new TestModuleThread(this->data);
	}

private:
	T data = NULL;
};

//显式实例化,可以把成员函数实现放到cpp，否则必须全部为内联
//template class TestModuleThread<int>;

#endif //__MODULE_THREAD_CXX11_H