#ifndef __MODULE_THREAD_CXX11_H
#define __MODULE_THREAD_CXX11_H
#pragma once
#include <thread>
#include <mutex>
#include <iostream>
#include "rwLock.h"



/*
class ModuleThread;
template<typename... Args>
void threadBody(ModuleThread* _this, Args&&... args);*/

class ModuleThread : public std::thread
{
public:
	//ModuleThread() noexcept : thread() {}

	//member function
	/*template<typename... Args>
	explicit
	ModuleThread(Args&&... args) :
		std::thread(&ModuleThread::threadBody, this, std::forward<Args>(args)...){}*/

		//friend
		/*template<typename... Args>
		explicit
		ModuleThread(Args&&... args) :
				std::thread(threadBody, this, std::forward<Args>(args)...) {}*/


	ModuleThread(int args) :
		std::thread(&ModuleThread::threadBody, this, args) {}





	/*template<typename Callable, typename... Args>
	explicit
	ModuleThread(Callable&& func, Args&&... args) :
		std::thread(std::forward<Callable>(func), std::forward<Args>(args)...) {}*/

	virtual ~ModuleThread()
	{
		this->join();
		thread::~thread();
	}

	// disable copy constructors
	ModuleThread(ModuleThread&) = delete;
	ModuleThread(const ModuleThread&) = delete;
	ModuleThread& operator=(const ModuleThread&) = delete;

public:
	bool isRunning() const
	{
		if (threadStatus == THREAD_RESUME)
			return true;
		else
			return false;
	}
	bool isFinished() const
	{
		if (threadStatus == THREAD_QUIT)
			return true;
		else
			return false;
	}

	void start()
	{
		resume();
	}

	//terminates the execution of the thread
	void terminate()
	{
		if (!isRunning())
		{
			resume();
		}
		//quit = true;
		std::unique_lock<std::mutex> ulk(counterMutex);
		threadStatus = THREAD_QUIT;
	}

	//suspend the thread
	void suspend()
	{
		if (threadStatus == THREAD_SUSPEND) { return; }

		std::unique_lock<std::mutex> ulk(counterMutex);//unique_lock退出作用域会自动解锁
		threadStatus = THREAD_SUSPEND;
		//cond.notify_one();//不应该唤醒任何线程
		std::cout << __LINE__ << " suspend." << std::endl; std::cout.flush();
	}

	//resume the thread
	void resume()
	{
		if (threadStatus == THREAD_RESUME) { return; }

		std::unique_lock<std::mutex> ulk(counterMutex);
		threadStatus = THREAD_RESUME;
		cond.notify_one();
		std::cout << __LINE__ << " resume." << std::endl; std::cout.flush();
	}

#if 0 //be replaced of suspend() and resume()
	//pause the action using pause flag
	void pause() { _pause = true; }
	virtual void stream() = 0; //stream the action using pause flag
#endif

private:
	void initThreadStatus()
	{
		std::unique_lock<std::mutex> ulk(counterMutex);
		//threads status
		threadStatus = THREAD_SUSPEND;
	}

	void checkThreadStatus()
	{
		std::unique_lock<std::mutex> ulk(counterMutex);
		while (threadStatus == THREAD_SUSPEND)
		{
			std::cout << __LINE__ << " suspend." << std::endl; std::cout.flush();
			cond.wait(ulk, [=]()->bool {return threadStatus == THREAD_RESUME; });//阻塞时会自动解锁ulk，唤醒时自动加锁
			std::cout << __LINE__ << " resume." << std::endl; std::cout.flush();
		}
	}

private:
	//the start point for the thread.
	//After calling start(), the created thread calls this function.
	/*template<typename... Args>
	void threadBody(Args&&... args)
	{
		//threads status
		initThreadStatus();

		while (threadStatus != THREAD_QUIT)
		{
			checkThreadStatus();//
			run(std::forward<Args>(args)...);
		}
	}*/


	/*template<typename... Args>
	friend void threadBody(ModuleThread* _this, Args&&... args)
	{
		//threads status
		_this->initThreadStatus();

		while (_this->threadStatus != ModuleThread::THREAD_QUIT)
		{
			_this->checkThreadStatus();//
			_this->run(std::forward<Args>(args)...);
		}
	}*/

	void threadBody(int args)
	{
		//threads status
		initThreadStatus();
		std::cout << __LINE__ << std::endl; std::cout.flush();
		while (threadStatus != THREAD_QUIT)
		{
			checkThreadStatus();//
			std::cout << __LINE__ << std::endl; std::cout.flush();
			run(args);
		}

		/*//std::unique_lock<std::mutex> ulk(counterMutex);
		int _threadStatus = THREAD_SUSPEND;
		do 
		{
			checkThreadStatus();//
			//std::cout << __LINE__ << std::endl; std::cout.flush();
			run(args);
			//_threadStatus =
		} while (_threadStatus != THREAD_QUIT);*/
	}

protected:
	// the task of the thread, which is without endless loop, called by threadBody().
	//run() is a virtual function, so that its refactoring function in subclass
	//will be called first.
	//virtual void run();
	virtual void run(int x)
	{
		std::cout << __LINE__ << std::endl;
		std::cout << "base: " << x << std::endl; std::cout.flush();
	}
	/*template<typename... Args>
	void run(Args ...args);*/

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
	std::mutex counterMutex;
	std::condition_variable cond;


private:
	enum THREAD_STATUS
	{
		THREAD_SUSPEND = 0,
		THREAD_RESUME = 1,
		THREAD_QUIT = 2
	};

	volatile int threadStatus = THREAD_SUSPEND;
};

class TestThresh : public ModuleThread
{
public:
	/*template<typename... Args>
	explicit
		TestThresh(Args&&... args) :
		ModuleThread(std::forward<Args>(args)...) {}*/
	TestThresh(int args):ModuleThread(args){}

	virtual ~TestThresh() {};

	/*void run(int &x, float &y)
	{
		y = x + y;
		std::cout << "x: " << x++ << "y: " << y << std::endl;
	}*/
	void run(int x)
	{
		std::cout << "Test: " << x << std::endl; std::cout.flush();
	}


private:

};


#endif //__MODULE_THREAD_CXX11_H
