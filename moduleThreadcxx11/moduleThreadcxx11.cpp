#include "moduleThreadcxx11.h"
#include "sleep.h"
#include "debugMsg.h"
#include <sstream>

ModuleThread::~ModuleThread()
{
	if (!isFinished())
	{
		stop();
		//TSleep::msleep(2);//wait for stopping.
	}
	this->join();
	thread::~thread();
}

void ModuleThread::start()
{
	resume();
}

void ModuleThread::stop()
{
	if (isFinished())
		return;
	//quit = true;
	std::unique_lock<std::mutex> ulk(counterMutex);
	threadStatus = MODULE_THREAD_QUIT;
	cond.notify_one();
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "stop request.");
	MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "stop request.");
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
		return true;
	}
	else
		return false;
}


//suspend the thread
void ModuleThread::suspend()
{
	if (threadStatus == MODULE_THREAD_SUSPEND) { return; }

	std::unique_lock<std::mutex> ulk(counterMutex);//unique_lock�˳���������Զ�����
	threadStatus = MODULE_THREAD_SUSPEND;
	//cond.notify_one();//��Ӧ�û����κ��߳�
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "suspend request.");
	MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "suspend request.");
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
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "resume request.");
	MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "resume request.");
}

void ModuleThread::checkThreadStatus()
{
	std::unique_lock<std::mutex> ulk(counterMutex);
	while (threadStatus == MODULE_THREAD_SUSPEND)
	{
		//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "suspended.");
		MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "suspended.");
		cond.wait(ulk, [=]()->bool {
				return (threadStatus == MODULE_THREAD_RESUME || 
					threadStatus == MODULE_THREAD_QUIT);}
		);//����ʱ���Զ�����ulk������ʱ�Զ�����
		//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "resumed.");
		MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "resumed.");
	}
}

void ModuleThread::run()
{
	//�๹��ʱ�������counterMutex������δ�������
	//���๹���ͬʱ�����˸��̺߳���
	//��Ҫsleepһ��ʱ�䣬�ȴ����Ա�������,���Թ�1����Ҳ�㹻
	TSleep::msleep(10);
	while(true)
	{
		TSleep::msleep(200);
		checkThreadStatus();
		if (isFinished())
		{
			//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "finished.");
			MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "finished.");
			break;
		}
			
		callBackFuc();
	}
}


std::string ModuleThread::getTid()
{
	const std::thread::id  id = this->get_id();
	std::stringstream sin;
	sin << id;
	return sin.str();
}

std::string ModuleThread::getThisTid()
{
	const std::thread::id id = std::this_thread::get_id();
	std::stringstream sin;
	sin << id;
	return sin.str();
}