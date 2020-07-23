#ifndef __TEST_MODULE_THREAD_H
#define __TEST_MODULE_THREAD_H

#include "moduleThreadcxx11.h"
#include "debugMsg.h"

template<typename T>
class TestModuleThread :
	public ModuleThread
{
public:

	TestModuleThread() noexcept : ModuleThread(){}

	explicit
	TestModuleThread(/*const*/ T &_data, bool _loop = true)
		: ModuleThread(_loop), data(&_data), loop(_loop)
	{}

	virtual ~TestModuleThread() {}

	// disable copy constructors
	TestModuleThread(const TestModuleThread&) = delete;
	TestModuleThread& operator=(const TestModuleThread&) = delete;

private:
	//override for the thread body, which cannot contain endless loop
	void callBackFunc() override
	{
		WGuard wLock(this->rwLock);//write lock
		//RGuard rLock(this->rwLock);//read lock
		unsigned int lo = 10000;
		while (lo)
		{
			//MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "Test:", (*this->data)++);
			(*this->data)++;
			lo--;
		}
	}

	//override for restarting the thread after stop() 
	//create an object as same as that before stop()
	ModuleThread* restoreObject() override
	{
		return new TestModuleThread(*this->data, this->loop);
	}

private:
	T* data{ nullptr };
	bool loop{ true };
};

//��ʽʵ����,���԰ѳ�Ա����ʵ�ַŵ�cpp���������ȫ��Ϊ����
//template class TestModuleThread<int>;

#endif //__MODULE_THREAD_CXX11_H