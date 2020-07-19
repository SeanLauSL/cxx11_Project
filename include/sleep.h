#ifndef __SLEEP_H
#define __SLEEP_H

class TSleep
{
public:
	TSleep() {}
	~TSleep() {}
	/*
	@brief sleep for @t hour
	*/
	static void hsleep(int t)
	{
		std::this_thread::sleep_for(std::chrono::hours(t));
	}

	/*
	@brief sleep for @t minutes
	*/
	static void mnsleep(int t)
	{
		std::this_thread::sleep_for(std::chrono::minutes(t));
	}

	/*
	@brief sleep for @t seconds
	*/
	static void ssleep(int t)
	{
		std::this_thread::sleep_for(std::chrono::seconds(t));
	}

	/*
	@brief sleep for @t milliseconds
	*/
	static void msleep(int t)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(t));
	}

	/*
	@brief sleep for @t microseconds
	*/
	static void usleep(int t)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(t));
	}

	/*
	@brief sleep for @t nanoseconds
	*/
	static void nsleep(int t)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(t));
	}

};

#endif
