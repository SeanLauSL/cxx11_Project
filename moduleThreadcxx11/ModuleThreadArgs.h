#ifndef __MODULE_THREAD_ARGS_H
#define __MODULE_THREAD_ARGS_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <sstream>

#include "sleep.h"
#include "debugMsg.h"
#include "rwLock.h"

class ModuleThreadArgs :
	public std::thread
{
public:
	template<typename... Args>
	explicit
	ModuleThreadArgs(Args&&... args):
		std::thread(&ModuleThreadArgs::run, this, std::forward<Args>(args)...) {}

	virtual ~ModuleThreadArgs() 
	{
		if (!isFinished())
		{
			stop();
			TSleep::msleep(50);//wait for stopping.
		}
		this->join();
		thread::~thread();
		MSG_OUT(Msg::MSG_INFO, "\n");//
	}

	// disable copy constructors
	ModuleThreadArgs(const ModuleThreadArgs&) = delete;
	ModuleThreadArgs& operator=(const ModuleThreadArgs&) = delete;

public:
	bool isRunning() const
	{
		//std::unique_lock<std::mutex> ulk(counterMutex);
		if (threadStatus == MODULE_THREAD_RESUME)
			return true;
		else
			return false;
	}

	bool isFinished() const
	{
		//std::unique_lock<std::mutex> ulk(counterMutex);
		if (threadStatus == MODULE_THREAD_QUIT)
		{
			return true;
		}
		else
			return false;
	}

	void start()
	{
		resume();
	}

	//terminates the execution of the thread
	void stop()
	{
		if (isFinished())
			return;
		//quit = true;
		std::unique_lock<std::mutex> ulk(counterMutex);
		threadStatus = MODULE_THREAD_QUIT;
		cond.notify_one();
		MSG_OUT(Msg::MSG_INFO, "%s %s %s\n", "thread", getTidStr(get_id()).c_str(), "stop request.");
	}

	//suspend the thread
	void suspend()
	{
		if (threadStatus == MODULE_THREAD_SUSPEND) { return; }

		std::unique_lock<std::mutex> ulk(counterMutex);//unique_lock退出作用域会自动解锁
		threadStatus = MODULE_THREAD_SUSPEND;
		//cond.notify_one();//不应该唤醒任何线程
		MSG_OUT(Msg::MSG_INFO, "%s %s %s\n", "thread", getTidStr(get_id()).c_str(), "suspend request.");
	}

	//resume the thread
	void resume()
	{
		if (threadStatus == MODULE_THREAD_RESUME) { return; }

		std::unique_lock<std::mutex> ulk(counterMutex);
		//counterMutex.lock();
		//std::unique_lock<std::mutex> ulk(counterMutex, std::defer_lock);
		threadStatus = MODULE_THREAD_RESUME;
		cond.notify_one();
		MSG_OUT(Msg::MSG_INFO, "%s %s %s\n", "thread", getTidStr(get_id()).c_str(), "resume request.");
	}

#if 0 //be replaced of suspend() and resume()
	//pause the action using pause flag
	void pause() { _pause = true; }
	virtual void stream() = 0; //stream the action using pause flag
#endif

private:
	//void initThreadStatus();

	void checkThreadStatus()
	{
		std::unique_lock<std::mutex> ulk(counterMutex);
		while (threadStatus == MODULE_THREAD_SUSPEND)
		{
			MSG_OUT(Msg::MSG_INFO, "%s %s %s\n", "thread", getTidStr(get_id()).c_str(), "suspended.");
			cond.wait(ulk, [=]()->bool {
				return (threadStatus == MODULE_THREAD_RESUME ||
					threadStatus == MODULE_THREAD_QUIT); }
			);//阻塞时会自动解锁ulk，唤醒时自动加锁
			MSG_OUT(Msg::MSG_INFO, "%s %s %s\n", "thread", getTidStr(get_id()).c_str(), "resumed.");
		}
	}

	//get Thread Id Of String
	std::string getTidStr(const std::thread::id & id)
	{
		std::stringstream sin;
		sin << id;
		return sin.str();
	}


private:
	//the start point for the thread.
	//After calling start(), the created thread calls this function.
	void run(int x, int y)
	{
		//类构造时，里面的counterMutex变量还未构造完成
	//且类构造的同时构造了该线程函数
	//需要sleep一段时间，等待类成员构造完成,测试过1纳秒也足够
		TSleep::msleep(10);
		while (true)
		{
			TSleep::msleep(200);
			checkThreadStatus();
			if (isFinished())
			{
				MSG_OUT(Msg::MSG_INFO, "%s %s %s\n", "thread", getTidStr(get_id()).c_str(), "finished.");
				break;
			}

			callBackFuc(x, y);
		}
	}


protected:
	// the task of the thread, which is without endless loop, called by run().
	//callBackFuc() is a virtual function, so that its refactoring function in subclass
	//will be called first.
	virtual void callBackFuc(int x, int y)
	{
		std::cout << __LINE__ << " base: " << std::endl; std::cout.flush();
	}

protected:
	//bool quit;
	//bool _pause;
	//read-write lock (be used in a subclass of ModuleThread)
	//WfirstRWLock rwLock;

#if 0 //调用方式
	void wfuc()
	{
		WriteGuard<WfirstRWLock> wLock(rwLock);
		//...
	}
	//离开函数范围自动解writeGuard锁,不会产生死锁，无需处理异常退出清理线程的操作	
#endif


private:
	mutable std::mutex counterMutex; //mutable for function isFinished()
	std::condition_variable cond;


private:
	enum THREAD_STATUS
	{
		MODULE_THREAD_SUSPEND = 0,
		MODULE_THREAD_RESUME = 1,
		MODULE_THREAD_QUIT = 2
	};

	volatile int threadStatus{ MODULE_THREAD_SUSPEND };
};


#endif