#include "moduleThreadcxx11.h"
#include "sleep.h"
#include "debugMsg.h"
#include <sstream>
#include <memory> //std::share_ptr

ModuleThread::~ModuleThread()
{
	MSG_ARGS(Msg::MSG_INFO, "releasing thread", getTid());
	if (!isFinished()){
		stop();
	}
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
	std::unique_lock<std::mutex> ulk(cMutex);
	threadStatus = MODULE_THREAD_QUIT;
	cdv.notify_one();
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "stop request.");
	MSG_ARGS(Msg::MSG_INFO, "thread", getTid(), "stop request.");
	exTid = getTid();
	if (thr.joinable()){
		thr.join();
	}
}

bool ModuleThread::isRunning() const
{
	//std::unique_lock<std::mutex> ulk(cMutex);
	if (threadStatus == MODULE_THREAD_RESUME)
		return true;
	else
		return false;
}

bool ModuleThread::isFinished() const
{
	//std::unique_lock<std::mutex> ulk(cMutex);
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

	std::unique_lock<std::mutex> ulk(cMutex);//unique_lock退出作用域会自动解锁
	threadStatus = MODULE_THREAD_SUSPEND;
	//cdv.notify_one();//不应该唤醒任何线程
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "suspend request.");
	MSG_ARGS(Msg::MSG_INFO, "thread", getTid(), "suspend request.");
}

//resume the thread
void ModuleThread::resume()
{
	if (threadStatus == MODULE_THREAD_RESUME) { return; }

	std::unique_lock<std::mutex> ulk(cMutex);
	//cMutex.lock();
	//std::unique_lock<std::mutex> ulk(cMutex, std::defer_lock);
	threadStatus = MODULE_THREAD_RESUME;
	cdv.notify_one();
	//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "resume request.");
	MSG_ARGS(Msg::MSG_INFO, "thread", getTid(), "resume request.");
}

void ModuleThread::checkThreadStatus()
{
	std::unique_lock<std::mutex> ulk(cMutex);
	while (threadStatus == MODULE_THREAD_SUSPEND)
	{
		//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "suspended.");
		MSG_ARGS(Msg::MSG_INFO, "thread", getTid(), "suspended.");
		cdv.wait(ulk, [=]()->bool {
				return (threadStatus == MODULE_THREAD_RESUME || 
					threadStatus == MODULE_THREAD_QUIT);}
		);//阻塞时会自动解锁ulk，唤醒时自动加锁
		//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "resumed.");
		MSG_ARGS(Msg::MSG_INFO, "thread", getTid(), "resumed.");
	}
}

void ModuleThread::run()
{
	//wait for the construction of std::mutex and std::condition_variable
	//TSleep::msleep(10);
	do{
		//MSG_ARGS(Msg::MSG_INFO, "thread", getTid().c_str(), "fps.", maxFps);
		TSleep::msleep(1000 / maxFps);
		checkThreadStatus();
		if (isFinished())
		{
			//MSG_PRINTF(Msg::MSG_INFO, "%s %s %s\n", "thread", getTid().c_str(), "finished.");
			MSG_ARGS(Msg::MSG_INFO, "thread", getTid(), "finished.");
			break;
		}
		//pure virtual function. a default method should be provided if we want to call it
		callBackFunc();
	} while (loop);
}

/*
//a default method
void ModuleThread::callBackFunc()
{
	MSG_ARGS(Msg::MSG_WARNING, "[warning]:", 
		" callBackFunc() of based class ModuleThread was called.");
}*/


WfirstRWLock & ModuleThread::t_rwLock()
{
	//initial a write-read-lock while calling t_rwLock() at the 1st time 
	static WfirstRWLock rwLock;
	return rwLock;
}


void ModuleThread::restart()
{
	if (!isFinished()){
		stop();
	}
	thr = std::thread(&ModuleThread::run, this);
	MSG_ARGS(Msg::MSG_WARNING, "[warning]:",
		"thread", exTid, "was restarted as",
		"thread", getTid());
	resume();
}


std::string ModuleThread::getTid()
{
	const std::thread::id  id = thr.get_id();
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

void ModuleThread::setMaxFps(const unsigned int mfps)
{
	if (mfps > 0 && mfps < 1000)
	{
		maxFps = mfps;
	}
}