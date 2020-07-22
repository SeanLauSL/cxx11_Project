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
			MSG_ARGS(Msg::MSG_DEBUG, "thread", ModuleThread::getThisTid().c_str(), i);
			//MSG_PRINTF(Msg::MSG_DEBUG, "%s %s %d\n", " thread ", ModuleThread::getThisTid().c_str(), i);
			TSleep::msleep(100);
		}
	}

public:
	using mt = ModuleThread;
	using test_mt = TestModuleThread<int>;
	using shptr_mt = std::shared_ptr<ModuleThread>;
	//ModuleThread* tModule;
	shptr_mt tModule;
	int inputData{ 5 };

private:

};

using std::cout;
using std::endl;

void Project::run()
{
	tModule = shptr_mt(new test_mt(inputData));//tmt -> ModuleThread* 多态
	//tmt* accp = new test_mt(inputData);//常规
	TSleep::msleep(250);//wait for construction
	tModule->setMaxFps(10);
	tModule->start();//start tModule
	MSG_ARGS(Msg::MSG_INFO, "main thread", ModuleThread::getThisTid().c_str());
	extraWork();
	tModule->suspend();
	tModule->stop();
	//extraWork
	int n = 0;
	std::cin >> n;
	//restart tModule
	tModule.reset(tModule->getRestartThread());
	extraWork();
	tModule->suspend();
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s\n", " thread ", ModuleThread::getThisTid().c_str());
	MSG_ARGS(Msg::MSG_INFO, "main thread", ModuleThread::getThisTid().c_str());
}

int main()
{
	Project pro;
	pro.run();
	TSleep::msleep(2000);
	return 0;
}

