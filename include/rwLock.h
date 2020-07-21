#ifndef __RW_LOCK_H
#define __RW_LOCK_H

#include <mutex>
#include <condition_variable>

class WfirstRWLock
{
public:
	WfirstRWLock() = default;
	~WfirstRWLock() = default;
public:
	void readLock()
	{
		std::unique_lock<std::mutex> ulk(counter_mutex);//unique_lock退出作用域会自动解锁
		cond_r.wait(ulk, [=]()->bool {return write_cnt == 0; });
		++read_cnt;//
	}
	void writeLock()
	{
		std::unique_lock<std::mutex> ulk(counter_mutex);
		++write_cnt;//等待前就++，会阻止新的读进程；write_cnt挪到wait后面，cond_r挪到wait前面就是读优先了
		cond_w.wait(ulk, [=]()->bool {return read_cnt == 0 && !inwriteflag; });
		inwriteflag = true;
	}
	void readUnlock()
	{
		std::unique_lock<std::mutex> ulk(counter_mutex);
		if (--read_cnt == 0 && write_cnt > 0)
		{
			cond_w.notify_one();
		}
	}
	void writeUnlock()
	{
		std::unique_lock<std::mutex> ulk(counter_mutex);
		if (--write_cnt == 0)
		{
			cond_r.notify_all();
		}
		else
		{
			cond_w.notify_one();
		}
		inwriteflag = false;
	}

private:
	volatile size_t read_cnt{ 0 };
	volatile size_t write_cnt{ 0 };
	volatile bool inwriteflag{ false };
	std::mutex counter_mutex;
	std::condition_variable cond_w;
	std::condition_variable cond_r;
};

//RAII method

template <typename _RWLockable>
class WriteGuard
{
public:
	explicit WriteGuard(_RWLockable &_rwLockable)
		: rwLockable(_rwLockable)
	{
		rwLockable.writeLock();
	}
	~WriteGuard()
	{
		rwLockable.writeUnlock();
	}
private:
	WriteGuard() = delete;
	WriteGuard(const WriteGuard&) = delete;
	WriteGuard& operator=(const WriteGuard&) = delete;
private:
	_RWLockable &rwLockable;
};

template <typename _RWLockable>
class ReadGuard
{
public:
	explicit ReadGuard(_RWLockable &_rwLockable)
		: rwLockable(_rwLockable)
	{
		rwLockable.readLock();
	}
	~ReadGuard()
	{
		rwLockable.readUnlock();
	}
private:
	ReadGuard() = delete;
	ReadGuard(const ReadGuard&) = delete;
	ReadGuard& operator=(const ReadGuard&) = delete;
private:
	_RWLockable &rwLockable;
};

/*
//reference https://blog.csdn.net/mymodian9612/article/details/52794980
*/

#endif
