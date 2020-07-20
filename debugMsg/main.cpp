/*
@seanlau

*/
#include "debugMsg.h"
#include "enableIf.hpp"

int main()
{
	using std::endl;

	int ar = 5;
	const int le = 1;
	
	//test<flag>(ar);
	Debug::test<Debug::INFO>("msg");
	Debug::test<2, int>(ar + 2);
	Debug::test<3, int>(3);
	Debug::test<4>("2.");
	Debug::test<6, int, float>(1, 2.6f);

	Msg::setLevel(Msg::MSG_INFO);//INFOLEVEL及以上的级别可以输出
	//Msg::setSeperator("+");//分隔符
	Msg::setFullpath(true);//display fullpath

	//__FILE__ and __LINE__ is not correct without #define
	Msg(Msg::MSG_INFO) << "msg" << " construction" << std::endl;

	MSG_PRINTF(Msg::MSG_INFO, "%s %d\n", "str", 50);
	MSG_OUT(Msg::MSG_INFO) << "test " << "str " << "out" << std::endl;
	MSG_ARGS(3, "str", "args", 6.5, 3);
	MSG_ARGS(Msg::MSG_INFO, "str", "enum", "info");
	//MSG_ARGS("K", "str", "enum", "info");
	
	
	return 0;

}
