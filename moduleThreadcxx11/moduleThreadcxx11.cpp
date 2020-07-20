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

	std::unique_lock<std::mutex> ulk(counterMutex);//unique_lock退出作用域会自动解锁
	threadStatus = MODULE_THREAD_SUSPEND;
	//cond.notify_one();//不应该唤醒任何线程
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
		);//阻塞时会自动解锁ulk，唤醒时自动加锁
		//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "resumed.");
		MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "resumed.");
	}
}

void ModuleThread::run()
{
	//类构造时，里面的counterMutex变量还未构造完成
	//且类构造的同时构造了该线程函数
	//需要sleep一段时间，等待类成员构造完成,测试过1纳秒也足够
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