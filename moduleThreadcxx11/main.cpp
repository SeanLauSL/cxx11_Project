/*
@seanlau

*/
//#include <thread>
//#include "moduleThreadcxx11.h"
#include "TestModuleThread.h"
#include "sleep.h"
//#include "test.h"
//#include "debugMsg.h"

int main()
{
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

	using tmt = TestModuleThread<int>;
	ModuleThread* accp = new tmt(inputData);
	accp->start();
	//TSleep::msleep(1000);
	//accp->suspend();
	//accp->stop();
	std::thread::id id = std::this_thread::get_id();
	for (int i = 0; i < 10; i++)
	{
		std::cout << __FILE__ << __LINE__ << " thread "<< std::this_thread::get_id() <<" "<< i << std::endl;
		TSleep::msleep(100);
	}
	accp->suspend();
	int n;
	std::cin >> n;


	delete (tmt *)accp;

	//MSG_OUT(Msg::MSG_INFO, "%s\n", "main");
	std::cout << __FILE__<<__LINE__ << " thread " << std::this_thread::get_id() << std::endl;
	return 0;

}

