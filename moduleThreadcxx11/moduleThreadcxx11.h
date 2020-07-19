#ifndef __MODULE_THREAD_CXX11_H
#define __MODULE_THREAD_CXX11_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "rwLock.h"


/*
class ModuleThread;
template<typename... Args>
void run(ModuleThread* _this, Args&&... args);*/

class ModuleThread : public std::thread
{
public:
	//ModuleThread() noexcept : thread() {}

	//member function
	/*template<typename... Args>
	explicit
	ModuleThread(Args&&... args) :
		std::thread(&ModuleThread::run, this, std::forward<Args>(args)...){}*/

		//friend
		/*template<typename... Args>
		explicit
		ModuleThread(Args&&... args) :
				std::thread(run, this, std::forward<Args>(args)...) {}*/

	explicit ModuleThread(int args);





	/*template<typename Callable, typename... Args>
	explicit
	ModuleThread(Callable&& func, Args&&... args) :
		std::thread(std::forward<Callable>(func), std::forward<Args>(args)...) {}*/

	virtual ~ModuleThread();

	// disable copy constructors
	ModuleThread(const ModuleThread&) = delete;
	ModuleThread& operator=(const ModuleThread&) = delete;

public:
	bool isRunning() const;
	bool isFinished() const;

	void start();

	//terminates the execution of the thread
	void stop();

	//suspend the thread
	void suspend();

	//resume the thread
	void resume();

#if 0 //be replaced of suspend() and resume()
	//pause the action using pause flag
	void pause() { _pause = true; }
	virtual void stream() = 0; //stream the action using pause flag
#endif

private:
	//void initThreadStatus();

	void checkThreadStatus();

private:
	//the start point for the thread.
	//After calling start(), the created thread calls this function.
	/*template<typename... Args>
	void run(Args&&... args)
	{
		//threads status
		initThreadStatus();

		while (threadStatus != MODULE_THREAD_QUIT)
		{
			checkThreadStatus();//
			callBackFuc(std::forward<Args>(args)...);
		}
	}*/


	/*template<typename... Args>
	friend void run(ModuleThread* _this, Args&&... args)
	{
		//threads status
		_this->initThreadStatus();

		while (_this->threadStatus != ModuleThread::MODULE_THREAD_QUIT)
		{
			_this->checkThreadStatus();//
			_this->callBackFuc(std::forward<Args>(args)...);
		}
	}*/

	void run(int args);

protected:
	// the task of the thread, which is without endless loop, called by run().
	//callBackFuc() is a virtual function, so that its refactoring function in subclass
	//will be called first.
	//virtual void callBackFuc();
	virtual void callBackFuc(int x)
	{
		std::cout << __LINE__  << " base: " << x << std::endl; std::cout.flush();
	}
	/*template<typename... Args>
	void callBackFuc(Args ...args);*/

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


class TestThresh : public ModuleThread
{
public:
	/*template<typename... Args>
	explicit
		TestThresh(Args&&... args) :
		ModuleThread(std::forward<Args>(args)...) {}*/
	TestThresh(int args):ModuleThread(args){}

	virtual ~TestThresh() { };

	/*void callBackFuc(int &x, float &y)
	{
		y = x + y;
		std::cout << "x: " << x++ << "y: " << y << std::endl;
	}*/
	void callBackFuc(int x)
	{
		std::cout << "Test: " << x << std::endl; std::cout.flush();
	}


private:

};


#endif //__MODULE_THREAD_CXX11_H
