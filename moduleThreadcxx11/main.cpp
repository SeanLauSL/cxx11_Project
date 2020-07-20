/*
@seanlau

*/
//#include <thread>
#include "TestModuleThread.hpp"
#include "sleep.h"
#include "debugMsg.h"

int main()
{
	using std::cout;
	using std::endl;

	//Msg::setLevel(Msg::MSG_INFO);
	/*mThread demo;
	demo.start();
	TSleep::msleep(1000 * 5);
	demo.suspend();*/

	/*TestThresh *tThresh = new TestThresh();
	tThresh->create(1, 2.5);*/

	
	/*TestThresh *tThresh = new TestThresh(1);
	tThresh->start();
	TSleep::msleep(1000);
	tThresh->suspend();*/

	//多态调用
	int inputData = 5;
	/*tc<int> tdm(inputData);
	tdm.ctm();*/

	Msg::setLevel(1);
	Msg::setFullpath(false);
	using tmt = TestModuleThread<int>;
	ModuleThread* accp = new tmt(inputData);
	accp->start();
	//TSleep::msleep(1000);
	
	std::thread::id id = std::this_thread::get_id();
	cout <<"main: "<< id << endl;
	
	for (int i = 0; i < 10; i++)
	{
		//MSG_OUT(Msg::MSG_DEBUG)<<"thread " << ModuleThread::getThisTid().c_str() << ": " << i << endl;
		MSG_ARGS(Msg::MSG_DEBUG, "thread", ModuleThread::getThisTid().c_str(), i);
		//MSG_PRINTF(Msg::MSG_DEBUG, "%s %s %d\n", " thread ", ModuleThread::getThisTid().c_str(), i);
		TSleep::msleep(100);
	}
	accp->suspend();
	//accp->stop();
	int n = 0;
	std::cin >> n;


	delete (tmt *)accp;

	//MSG_PRINTF(Msg::MSG_INFO, "%s %s\n", " thread ", ModuleThread::getThisTid().c_str());
	MSG_ARGS(Msg::MSG_INFO, "thread", ModuleThread::getThisTid().c_str());

	return 0;

}

