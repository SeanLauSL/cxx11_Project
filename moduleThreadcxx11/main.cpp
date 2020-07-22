/*
@seanlau

*/
//#include <thread>
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

public:
	using tmt = TestModuleThread<int>;
	ModuleThread* tModule;
	int inputData{ 5 };

private:

};

using std::cout;
using std::endl;

void Project::run()
{
	tModule = new tmt(inputData);//多态
	//tmt* accp = new tmt(inputData);//常规
	TSleep::msleep(250);//wait for construction
	tModule->setMaxFps(10);
	tModule->start();

	std::thread::id id = std::this_thread::get_id();
	cout << "main: " << id << endl;

	for (int i = 0; i < 10; i++)
	{
		//MSG_OUT(Msg::MSG_DEBUG)<<"thread " << ModuleThread::getThisTid().c_str() << ": " << i << endl;
		MSG_ARGS(Msg::MSG_DEBUG, "thread", ModuleThread::getThisTid().c_str(), i);
		//MSG_PRINTF(Msg::MSG_DEBUG, "%s %s %d\n", " thread ", ModuleThread::getThisTid().c_str(), i);
		TSleep::msleep(100);
	}
	tModule->suspend();
	tModule->stop();
	int n = 0;
	std::cin >> n;

	tModule = tModule->getRestartThread();
	for (int i = 0; i < 10; i++)
	{
		MSG_ARGS(Msg::MSG_DEBUG, "thread", ModuleThread::getThisTid().c_str(), i);
		TSleep::msleep(100);
	}
	tModule->suspend();

	delete (tmt *)tModule;

	//MSG_PRINTF(Msg::MSG_INFO, "%s %s\n", " thread ", ModuleThread::getThisTid().c_str());
	MSG_ARGS(Msg::MSG_INFO, "thread", ModuleThread::getThisTid().c_str());
	TSleep::msleep(2000);
}

int main()
{
	Project pro;
	pro.run();
	return 0;
}

