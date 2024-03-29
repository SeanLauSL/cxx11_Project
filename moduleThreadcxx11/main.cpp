/*
@seanlau

*/
#include <memory>
#include "TestModuleThread.hpp"
#include "sleep.h"
#include "debugMsg.h"

//namespace
using msg::Msg;
using mType = msg::MSG_LEVEL;

const int threadNum = 10;

class Project
{
public:
	Project()
	{
		Msg::setLevel(mType::MSG_INFO);
		Msg::setFullpath(false);
	}
	~Project() {};

public:
	void run();

private:
	void extraWork()
	{
		for (int i = 0; i < 10; i++)
		{
			//MSG_OUT(Msg::MSG_DEBUG)<<"thread " << ModuleThread::getThisTid().c_str() << ": " << i << endl;
			MSG_ARGS(mType::MSG_DEBUG, "thread", ModuleThread::getThisTid(), i);
			//MSG_PRINTF(Msg::MSG_DEBUG, "%s %s %d\n", " thread ", ModuleThread::getThisTid().c_str(), i);
			TSleep::msleep(100);
		}
	}

public:
	using mt = ModuleThread;
	using test_mt = TestModuleThread<int>;
	using shptr_mt = std::shared_ptr<ModuleThread>;
	//unsigned int threadNum = 2;
	//shptr_mt *tmdptr = new shptr_mt[threadNum];
	shptr_mt tmdptr[threadNum]; //array of shptr_mt(s)
	int inputData{ 5 };
	int ExtraInputData{ 10 };

private:

};

using std::cout;
using std::endl;

void Project::run()
{
	//start threadNum threads using share_ptr
	for (int i = 0; i < threadNum; i++)
	{
		if (i == threadNum-1)
		{
			tmdptr[i] = shptr_mt(new test_mt(ExtraInputData, false));
		}
		else
			tmdptr[i] = shptr_mt(new test_mt(inputData, false));
		
		tmdptr[i]->setMaxFps(10);
		tmdptr[i]->start();
	}
	//TSleep::ssleep(2);
	extraWork();// extra work on main thread
	//suspend and stop the threads
	for (int i = 0; i < threadNum; i++)
	{
		tmdptr[i]->suspend();
		//tmdptr[i]->stop();
	}
	int n = 0;
	std::cin >> n;//pause
	//restart the threads
	for (int i = 0; i < threadNum; i++)
	{
		tmdptr[i]->restart();
	}
	extraWork();// extra work on main thread
	//suspend the threads
	for (int i = 0; i < threadNum; i++)
	{
		tmdptr[i]->suspend();
	}
	MSG_ARGS(mType::MSG_INFO, "main thread", ModuleThread::getThisTid());
	/*
	//share_ptr auto do this
	for (int i = 0; i < threadNum; i++)
	{
		tmdptr[i].reset();
	}*/
}


int main()
{
	Project pro;
	MSG_ARGS(mType::MSG_INFO, "main thread", ModuleThread::getThisTid());
	pro.run();
	MSG_ARGS(mType::MSG_INFO, "main thread", ModuleThread::getThisTid(),
		"pro.inputData", pro.inputData, "ExtraInputData", pro.ExtraInputData);
	TSleep::msleep(2000);
	return 0;
}

