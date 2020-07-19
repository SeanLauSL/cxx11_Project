#pragma once
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

class mThread :public std::thread {
public:
	mThread() : thread(&mThread::run, this, 5) { this->detach(); }

	virtual ~mThread() {
		is_live = false; 
		/*if (this->isRunning())
		{
			stop();
		}
		this->join();
		std::thread::~thread();*/
	}
public:
	void thread_callback() {
		cout << "mThread run" << endl;
	}

	void run(int a);
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


private:
	void initThreadStatus();

	void checkThreadStatus();

private:
	bool is_live = true;

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

void mThread::run(int a) {
	//is_live = true;
	//cout << a << endl;
	while (!isFinished()) {
		checkThreadStatus();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		thread_callback();
	}
}

void mThread::start()
{
	initThreadStatus();
	resume();
}

void mThread::stop()
{
	if (isFinished())
		return;

	if (!isRunning())
	{
		resume();
	}
	//quit = true;
	std::unique_lock<std::mutex> ulk(counterMutex);
	threadStatus = MODULE_THREAD_QUIT;
	//counterMutex.unlock();
}

bool mThread::isRunning() const
{
	//std::unique_lock<std::mutex> ulk(counterMutex);
	if (threadStatus == MODULE_THREAD_RESUME)
		return true;
	else
		return false;
}


bool mThread::isFinished() const
{
	//std::unique_lock<std::mutex> ulk(counterMutex);
	if (threadStatus == MODULE_THREAD_QUIT)
		return true;
	else
		return false;
}


//suspend the thread
void mThread::suspend()
{
	if (threadStatus == MODULE_THREAD_SUSPEND) { return; }

	std::unique_lock<std::mutex> ulk(counterMutex);//unique_lock退出作用域会自动解锁
	threadStatus = MODULE_THREAD_SUSPEND;
	//cond.notify_one();//不应该唤醒任何线程
	std::cout << __LINE__ << " suspend request." << std::endl; std::cout.flush();
}

//resume the thread
void mThread::resume()
{
	if (threadStatus == MODULE_THREAD_RESUME) { return; }

	std::unique_lock<std::mutex> ulk(counterMutex);
	//counterMutex.lock();
	//std::unique_lock<std::mutex> ulk(counterMutex, std::defer_lock);
	threadStatus = MODULE_THREAD_RESUME;
	cond.notify_one();
	std::cout << __LINE__ << " resume." << std::endl; std::cout.flush();
}

void mThread::initThreadStatus()
{
	std::unique_lock<std::mutex> ulk(counterMutex);
	//threads status
	threadStatus = MODULE_THREAD_SUSPEND;
}

void mThread::checkThreadStatus()
{
	std::unique_lock<std::mutex> ulk(counterMutex);
	while (threadStatus == MODULE_THREAD_SUSPEND)
	{
		std::cout << __LINE__ << " suspend." << std::endl; std::cout.flush();
		cond.wait(ulk, [=]()->bool {return threadStatus == MODULE_THREAD_RESUME; });//阻塞时会自动解锁ulk，唤醒时自动加锁
		std::cout << __LINE__ << " resume." << std::endl; std::cout.flush();
	}
}