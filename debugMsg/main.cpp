/*
@seanlau

*/
#include "debugMsg.h"

int main()
{
	int ar = 5;
	const int le = 1;
	
	//test<flag>(ar);
	Debug::test<Debug::INFO>("msg");
	Debug::test<2, int>(ar + 2);
	Debug::test<3, int>(3);
	Debug::test<4>("2.");
	Debug::test<6, int, float>(1, 2.6);

	Msg::setLevel(Msg::MSG_INFO);//INFOLEVEL及以上的级别可以输出

	Msg(Msg::MSG_DEBUG) << "msg" << std::endl;

	MSG_OUT(Msg::MSG_INFO, "%s\n", "str");//use this
	
	
	return 0;

}
