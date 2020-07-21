#ifndef __MODULE_THREAD_CXX11_H
#define __MODULE_THREAD_CXX11_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "rwLock.h"


class ModuleThread : public std::thread
{
public:
	ModuleThread() noexcept : std::thread(&ModuleThread::run, this) {}

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

	//reconstructing the thread after stop() or suspend()
	ModuleThread* getRestartThread();

#if 0 //be replaced of suspend() and resume()
	//pause the action using pause flag
	void pause() { _pause = true; }
	virtual void stream() = 0; //stream the action using pause flag
#endif

	//get Thread Id Of which calls this function
	static std::string getThisTid();

	//get Thread Id Of current class in string
	std::string getTid();//const std::thread::id & id

	//set the max fps of this thread, which supposes at (0, 1000]
	void setMaxFps(const unsigned int mfps);

private:
	//void initThreadStatus();

	void checkThreadStatus();

	//the start point for the thread.
	//After calling start(), the created thread calls this function.
	void run();


protected:
	// the task of the thread, which is without endless loop, called by run().
	//callBackFuc() is a virtual function, so that its overrided function in subclass
	//will be called first.
	virtual void callBackFuc() = 0;

	//pure virtual function for restoring an object as same as that before stop() or suspend()
	//called by restart()
	virtual ModuleThread* restoreObject() = 0;

protected:
	//bool quit;
	//bool _pause;

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
	WfirstRWLock rwLock;

	using WGuard = WriteGuard<WfirstRWLock>;
	using RGuard = ReadGuard<WfirstRWLock>;

	int maxFps{ 60 };

private:
	std::mutex counterMutex; 
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

#endif //__MODULE_THREAD_CXX11_H
