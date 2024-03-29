/*
@seanlau

*/
#include "debugMsg.h"
#include "enableIf.hpp"

using msg::Msg;
using mType = msg::MSG_LEVEL;
using std::endl;
using std::cout;
int main()
{
	const char *nam = "tes.txt";
	MSG_ARGS(mType::MSG_INFO, Msg::getExtName(nam));
	const char* ty = ".jpg";
	cout << ty << endl;
	cout << ty+1 << endl;
	CostTime clock;
	clock.tick();
	int ar = 5;
	const int le = 1;
	MSG_ARGS(mType::MSG_INFO, clock.tock(), "ms");
	//test<flag>(ar);
	Debug::test<Debug::INFO>("msg");
	Debug::test<2, int>(ar + 2);
	Debug::test<3, int>(3);
	Debug::test<4>("2.");
	Debug::test<6, int, float>(1, 2.6f);

	Msg::setLevel(mType::MSG_INFO);//INFOLEVEL及以上的级别可以输出
	//Msg::setSeperator("+");//分隔符
	Msg::setFullpath(true);//display fullpath

	//__FILE__ and __LINE__ is not correct without #define
	Msg(mType::MSG_INFO) << "msg" << " construction" << std::endl;

	MSG_PRINTF(mType::MSG_INFO, "%s %d\n", "str", 50);
	MSG_OUT(mType::MSG_INFO) << "test " << "str " << "out" << std::endl;
	//MSG_ARGS(3, "str", "args", 6.5, 3); // 3->mType error
	//MSG_ARGS(false, "str", "args", 6.5, 3); //false->mType error
	MSG_ARGS(mType::MSG_INFO, "str", "enum", "info");
	//MSG_ARGS("K", "str", "enum", "info");
	
	MSG_ARGS(mType::MSG_INFO, Msg::fullpath2ShortName("abc.txt"), 
		Msg::getNameWtExt("abc.txt"));
	return 0;

}
