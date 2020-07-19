#include "moduleThreadcxx11.h"
#include "sleep.h"
#include "debugMsg.h"

ModuleThread::ModuleThread(int args) :
	std::thread(&ModuleThread::run, this, args) 
{ 
	/*this->detach();*/
}

ModuleThread::~ModuleThread()
{
	if (!isFinished())
	{
		stop();
		TSleep::msleep(50);//wait for stopping.
	}
	this->join();
	thread::~thread();
	MSG_OUT(Msg::MSG_INFO, "\n");//
	//std::cout << __LINE__ << std::endl;
}

void ModuleThread::start()
{
	resume();
}

void ModuleThread::stop()
{
	if (isFinished())
		return;
	/*if (!isRunning())
	{
		std::cout << __LINE__ << " thread " << get_id() << " checked suspended. " << std::endl; std::cout.flush();
		resume();
		TSleep::msleep(50);//wait for resuming.
	}*/
	//quit = true;
	std::unique_lock<std::mutex> ulk(counterMutex);
	threadStatus = MODULE_THREAD_QUIT;
	cond.notify_one();
	MSG_OUT(Msg::MSG_INFO, "%s %d %s\n", "thread", get_id(), "stop request.");
	//std::cout << __LINE__ << " thread " << get_id() << " stop request." << std::endl; std::cout.flush();
}

bool ModuleThread::isRunning() const
{
	//std::unique_lock<std::mutex> ulk(counterMutex);
	if (threadStatus == MODULE_THREAD_RESUME)
		return true;
	else
		return false;
}

bool ModuleThread::isFinished() const
{
	//std::unique_lock<std::mutex> ulk(counterMutex);
	if (threadStatus == MODULE_THREAD_QUIT)
	{
		MSG_OUT(Msg::MSG_INFO, "%s %d %s\n", "thread", get_id(), "finished.");
		//std::cout << __LINE__ << " thread " << get_id() << " finished." << std::endl; std::cout.flush();
		return true;
	}
	else
		return false;
}

/*
bool ModuleThread::isFinished() const
{
	bool ret = false;
	if (threadStatus == MODULE_THREAD_QUIT)
		ret = true;
	else
		ret = false;
	return ret;
}*/

//suspend the thread
void ModuleThread::suspend()
{
	if (threadStatus == MODULE_THREAD_SUSPEND) { return; }

	std::unique_lock<std::mutex> ulk(counterMutex);//unique_lock�˳���������Զ�����
	threadStatus = MODULE_THREAD_SUSPEND;
	//cond.notify_one();//��Ӧ�û����κ��߳�
	MSG_OUT(Msg::MSG_INFO, "%s %d %s\n", "thread", get_id(), "suspend request.");
	//std::cout << __LINE__ << " thread " << get_id() << " suspend request." << std::endl; std::cout.flush();
}

//resume the thread
void ModuleThread::resume()
{
	if (threadStatus == MODULE_THREAD_RESUME) { return; }

	std::unique_lock<std::mutex> ulk(counterMutex);
	//counterMutex.lock();
	//std::unique_lock<std::mutex> ulk(counterMutex, std::defer_lock);
	threadStatus = MODULE_THREAD_RESUME;
	cond.notify_one();
	MSG_OUT(Msg::MSG_INFO, "%s %d %s\n", "thread", get_id(), "resume request.");
	//std::cout << __LINE__ << " thread " << get_id() << " resume request." << std::endl; std::cout.flush();
}

void ModuleThread::checkThreadStatus()
{
	std::unique_lock<std::mutex> ulk(counterMutex);
	while (threadStatus == MODULE_THREAD_SUSPEND)
	{
		MSG_OUT(Msg::MSG_INFO, "%s %d %s\n", "thread", get_id(), "suspended.");
		//std::cout << __LINE__ << " thread "<< get_id() << " suspended." << std::endl; std::cout.flush();
		cond.wait(ulk, [=]()->bool {
				return (threadStatus == MODULE_THREAD_RESUME || 
					threadStatus == MODULE_THREAD_QUIT);}
		);//����ʱ���Զ�����ulk������ʱ�Զ�����
		//cond.wait(ulk, [=]()->bool {return (threadStatus == MODULE_THREAD_RESUME); });
		//std::cout << __LINE__ << " thread " << get_id() << " resumed." << std::endl; std::cout.flush();
		MSG_OUT(Msg::MSG_INFO, "%s %d %s\n", "thread", get_id(), "resumed.");
	}
}

void ModuleThread::run(int args)
{
	//�๹��ʱ�������counterMutex������δ�������
	//���๹���ͬʱ�����˸��̺߳���
	//��Ҫsleepһ��ʱ�䣬�ȴ����Ա�������
	//TSleep::msleep(10);
	while(true)
	{
		TSleep::msleep(200);//ʹ��ͬһ��sleep
		checkThreadStatus();//
		if(isFinished())
			break;
		callBackFuc(args);
	}
}
