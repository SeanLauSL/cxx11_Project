#ifndef __SLEEP_H
#define __SLEEP_H

class TSleep
{
public:
	TSleep() {}
	~TSleep() {}

	static void msleep(int n)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(n));
	}

};

#endif
