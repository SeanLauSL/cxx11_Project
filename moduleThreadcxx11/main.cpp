/*
@seanlau

*/
#include <memory>
#include "TestModuleThread.hpp"
#include "sleep.h"
#include "debugMsg.h"

class Project
{
public:
	Project()
	{
		Msg::setLevel(1);
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
			MSG_ARGS(Msg::MSG_DEBUG, "thread", ModuleThread::getThisTid(), i);
			//MSG_PRINTF(Msg::MSG_DEBUG, "%s %s %d\n", " thread ", ModuleThread::getThisTid().c_str(), i);
			TSleep::msleep(100);
		}
	}

public:
	using mt = ModuleThread;
	using test_mt = TestModuleThread<int>;
	using shptr_mt = std::shared_ptr<ModuleThread>;
	//ModuleThread* tModule;
	shptr_mt tModule, tModule_cand;
	int inputData{ 5 };

private:

};

using std::cout;
using std::endl;

void Project::run()
{
	tModule = shptr_mt(new test_mt(inputData, false));//tmt -> ModuleThread* 多态
	tModule_cand = shptr_mt(new test_mt(inputData, false));
	//tmt* accp = new test_mt(inputData);//常规
	//TSleep::msleep(250);//wait for construction
	tModule->setMaxFps(10);
	tModule_cand->setMaxFps(10);
	TSleep::msleep(10);
	tModule->start();//start tModule
	tModule_cand->start();
	MSG_ARGS(Msg::MSG_INFO, "main thread", ModuleThread::getThisTid());
	//extraWork();
	TSleep::ssleep(2);
	tModule->suspend();
	tModule->stop();
	tModule_cand->stop();
	//extraWork
	int n = 0;
	std::cin >> n;
	//restart tModule
	tModule->restart();
	tModule_cand->restart();
	extraWork();
	tModule->suspend();
	tModule_cand->suspend();
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s\n", " thread ", ModuleThread::getThisTid().c_str());
	MSG_ARGS(Msg::MSG_INFO, "main thread", ModuleThread::getThisTid());
}

int main()
{
	Project pro;
	pro.run();
	MSG_ARGS(Msg::MSG_INFO, "pro.inputData", pro.inputData);
	TSleep::msleep(2000);
	return 0;
}

