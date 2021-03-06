#ifndef __MODULE_THREAD_CXX11_H
#define __MODULE_THREAD_CXX11_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "rwLock.h"


class ModuleThread
{
public:
	ModuleThread() noexcept {}

	//@param {_loop}: true - call callBackFunc() endlessly
	//				false - call callBackFunc() only one times
	explicit
	ModuleThread(bool _loop) : loop(_loop){}

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

	//starts the thread after construction
	void start();

	//terminates the execution of the thread
	void stop();

	//suspend the thread
	void suspend();

	//resume the thread after suspend()
	void resume();

	//restart the thread {thr}
	//it will stop the unfinished current thread and 
	//create a new thread, 
	void restart();

	//get Thread Id Of which calls this function
	static std::string getThisTid();

	//get Thread Id Of current class in string
	std::string getTid();//const std::thread::id & id

	//set the max fps of this thread, which is supposed at the range of (0, 1000]
	void setMaxFps(const unsigned int mfps);
	

private:
	//void initThreadStatus();

	void checkThreadStatus();

	//the start point for the thread.
	//After calling start(), the created thread calls this function.
	//(cannot be overrided)
	virtual void run() final;


protected:
	// the task of the thread, which cannot contain endless loop, called by run().
	//callBackFunc() is a virtual function, so that its overrided function in subclass
	//will be called first.
	virtual void callBackFunc() = 0;


protected:

	/*read-write lock (be used in a subclass of ModuleThread)
	using in the way of:
	void wfuc(){
		WriteGuard<WfirstRWLock> wLock(rwLock);
		//do something
	}//end wfuc
	@note
	//auto unlock rwLock while wfuc() finished
	//do not need to clean the death lock after a exception. 
	*/
	static WfirstRWLock rwLock;

#if 0
	/*on one hand, it only initial a write-read-lock when t_rwLock() is called at the 1st time.
	on the other hand, it will not create any write-read-lock member if the subclass do not need.
	further more, it shares the same rwlock at the whole class.
	However, it`s not thread safe. This method should use with a lock. Obviously, we should not
	use another lock to lock our rwLock.*/
	//static WfirstRWLock & t_rwLock();
#endif

	using WGuard = WriteGuard<WfirstRWLock>;
	using RGuard = ReadGuard<WfirstRWLock>;

private:
	int maxFps{ 60 };
	std::mutex cMutex; 
	std::condition_variable cdv;
	bool loop{ true }; // true - endless loop for this thread
	std::thread thr; //thread object
	std::string exTid{ "0" };// last thread id



private:
	enum THREAD_STATUS
	{
		MODULE_THREAD_SUSPEND = 0,
		MODULE_THREAD_RESUME = 1,
		MODULE_THREAD_QUIT = 2
	};

	volatile int threadStatus{ MODULE_THREAD_SUSPEND };
};

#endif //__MODULE_THREAD_CXX11_H
