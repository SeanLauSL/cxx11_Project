/*
@seanlau

*/
#include <iostream>
#include "opencv2/opencv.hpp"
#include <string>
#include "deque.hpp"
#include "human.hpp"

int main()
{
	//std::string
	TQueueConcurrent<std::string> dequeTS;//deque_thread_save
	std::string t0 = "t0";
	std::string t1 = "t1";
	dequeTS.emplace_back("tt");
	dequeTS.emplace_back(t0);
	dequeTS.emplace_back(t1);
	dequeTS.insert(dequeTS.locate(2), "loc");
	std::cout << "dequeTS size: " << dequeTS.size() << std::endl;
	while (dequeTS.size())
	{
		std::cout << "dequeTS val: " << dequeTS.pop_front() << std::endl;
	}

	//cv::Mat
	TQueueConcurrent<cv::Mat> fifo_mat;
	cv::Mat mt = cv::Mat::ones(3, 3, CV_8UC1);
	fifo_mat.emplace_back(mt);
	std::cout << "fifo_mat size: " << fifo_mat.size() << std::endl;
	std::cout << "fifo_mat val: "<< std::endl << fifo_mat.pop_front() << std::endl;
	
	//class 多参数
	TQueueConcurrent<Human> dequeSN;
	const char* name = "man";
	dequeSN.emplace_back(10, name);//call Human(10, "man");
	std::cout << "dequeSN size: " << dequeSN.size() << std::endl;
	while (dequeSN.size())
	{
		dequeSN.pop_front().print();
	}

	return 0;	
}
