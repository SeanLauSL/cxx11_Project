#ifndef __DEBUG_MSG_H
#define __DEBUG_MSG_H
#include <iostream>
#include <initializer_list>
#include <string>//must include this for cout<<(the rvalue of string)
class Msg
{
public:
	//no judge for enum, using !isFloat for instead
	template<typename T>
	using isInt = std::is_integral<T>;
	template<typename T>
	using isFloat = std::is_floating_point<T>;

	template<typename T,
		typename = std::enable_if_t<!isFloat<T>::value>, //block float and which can not convert to int
		typename... Args
	>
	Msg(T level, Args&&... args) : currentLevel(level)
	{
		if (level >= levelThresh)
		{
#if __cplusplus >= 201402L
			//must include <string> for cout<<(the rvalue of string)
			expand([](auto i) {std::cout << i << Msg::seperator; }, args...);
#else
			expandCallBack(args...);
#endif
		}

	}

	///explicit Msg(const int level);
	//__FILE__ and __LINE__ is not correct without #define
	template<typename T,
		typename = std::enable_if_t<!isFloat<T>::value> //block float and which can not convert to int
	>
	explicit
	Msg(T level) : currentLevel(level){}

	

	//print
	#define MSG_PRINTF(level,fmt,...)\
    do {\
        if (level >= Msg::levelThresh) {\
			if(Msg::isFullpath){\
				printf("%s(%d): " fmt, __FILE__, __LINE__, ##__VA_ARGS__);fflush(stdout);\
			}\
            else{\
				printf("%s(%d): " fmt, Msg::fp2sn(__FILE__).c_str(), __LINE__, ##__VA_ARGS__);fflush(stdout);\
			}\
        }\
    }\
    while(0)

	#define MSG_OUT(level)\
    do {\
        if (level >= Msg::levelThresh) {\
			if(Msg::isFullpath){\
				std::cout << __FILE__ ;\
			}\
			else{\
				std::cout << Msg::fp2sn(__FILE__);\
			}\
			std::cout<< "(" << __LINE__ << "): "; std::cout.flush();\
        }\
    }\
    while(0);Msg(level)

	#define MSG_ARGS(level, ...)\
    do {\
        if (level >= Msg::levelThresh) {\
			if(Msg::isFullpath){\
				std::cout << __FILE__ ;\
			}\
			else{\
				std::cout << Msg::fp2sn(__FILE__);\
			}\
			std::cout<< "(" << __LINE__ << "): "; std::cout.flush();\
        }\
    }\
    while(0);Msg(level, ##__VA_ARGS__)

private:
	//expand args for lambda
	template<typename F, typename... Args>
	void expand(const F& f, Args&&...args)
	{
		std::initializer_list<int>{(f(std::forward<Args>(args)), 0)...};
		std::cout << std::endl;
	}

	//expand args for callback func
	template <typename T>
	void process(T t)
	{
		std::cout << t << Msg::seperator;//must include <string> for cout<<(the rvalue of string)
	}

	template<typename... Args>
	void expandCallBack(Args&&...args)
	{
		std::initializer_list<int>{(process(std::forward<Args>(args)), 0)...};
		std::cout << std::endl;
	}

private:
	
	 

public:

	template <typename T>
	Msg & operator<<(T & t)
	{
		if (currentLevel >= levelThresh)
		{
			std::cout << t; std::cout.flush();
		}
		return *this;
	}

	Msg & operator<<(const char * ccp);

	//operator for the function pointer of std::endl;
	Msg& operator<<(std::basic_ostream<char, std::char_traits<char>>&
		(*_Pfn)(std::basic_ostream<char, std::char_traits<char>>&));

	//set message level
	static void setLevel(int level);

	//set seperator of message
	static void setSeperator(const std::string &spt);

	//display fullpath or not
	static void setFullpath(bool flag);

	/*
	@brief replace strOld with strNew
	@param @strBig source string
	@param @strOld old element
	@param @strNew new element
	*/
	static void string_replace(std::string &strBig,
		const std::string &strOld,
		const std::string &strNew);

	/*
	@brief get short name from fullpath:
	@sample C:\Test\abc.xyz --> abc.xyz
	@sample /home/seanlau/Test/abc.xyz --> abc.xyz
	@param @strFullName source fullpath
	@return short name
	*/
	static std::string fullpath2ShortName(std::string strFullName);

	/*
	@brief get path whitout short name from fullpath:
	@sample C:\Test\abc.xyz --> C:\Test
	@sample /home/seanlau/Test/abc.xyz --> /home/seanlau/Test
	@param @strFullName source fullpath
	@return path whitout short name
	*/
	static std::string fullpath2Path(std::string strFullName);

	typedef std::string(*FP2SN)(std::string);
	static FP2SN fp2sn;//alias name of function fullpath2ShortName

public:
	enum MSG_LEVEL
	{
		MSG_DEBUG = 1,
		MSG_INFO = 2,
		MSG_WARNING = 3,
		MSG_ERROR = 4
	};
	static int levelThresh;
	static std::string seperator;
	static bool isFullpath;

private:
	int currentLevel{ 0 };

};

#endif //__DEBUG_MSG_H
