#include "debugMsg.h"
#include <sstream>
#include <vector>

using msg::Msg;
using msg::MSG_LEVEL;
//static member
MSG_LEVEL Msg::levelThresh = MSG_LEVEL::MSG_DEBUG;
std::string Msg::seperator = " ";
bool Msg::isFullpath = false;
Msg::FP2SN Msg::fp2sn = Msg::fullpath2ShortName;

/*Msg::Msg(const int level) : currentLevel(level)
{
}
*/
void Msg::setLevel(MSG_LEVEL level)
{
	levelThresh = level;
}

void Msg::setSeperator(const std::string &spt)
{
	seperator = spt;
}

void Msg::setFullpath(bool flag)
{
	isFullpath = flag;
}

Msg& Msg::operator<<(const char * ccp)
{
	if (currentLevel >= levelThresh)
	{
		std::cout << ccp;
	}
	return *this;
}


Msg& Msg::operator<<(std::basic_ostream<char, std::char_traits<char>>&
(*_Pfn)(std::basic_ostream<char, std::char_traits<char>>&))
{
	if (currentLevel >= levelThresh)
	{
		std::cout << _Pfn;
	}
	return *this;
}

void Msg::string_replace(std::string &strBig,
	const std::string &strOld,
	const std::string &strNew)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strOld.size();
	std::string::size_type dstlen = strNew.size();

	while ((pos = strBig.find(strOld, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strNew);
		pos += dstlen;
	}
}


std::string Msg::fullpath2ShortName(std::string strFullName)
{
	if (strFullName.empty())
	{
		return "";
	}

	string_replace(strFullName, "/", "\\");

	std::string::size_type iPos = strFullName.find_last_of('\\') + 1;

	return strFullName.substr(iPos, strFullName.length() - iPos);//(position, length)
}


std::string Msg::fullpath2Path(std::string strFullName)
{
	if (strFullName.empty())
	{
		return "";
	}

	string_replace(strFullName, "/", "\\");

	std::string::size_type iPos = strFullName.find_last_of('\\') + 1;

	return strFullName.substr(0, iPos);
}

std::string Msg::getNameWtExt(std::string str)
{
	std::string sName = fullpath2ShortName(str);
	std::stringstream ss(sName);
	std::string taken;
	std::getline(ss, taken, '.');
	return taken;
}

std::string Msg::getExtName(std::string str)
{
	std::string sName = fullpath2ShortName(str);
	std::stringstream ss(sName);
	std::string taken;
	std::vector<std::string> vec;
	while (std::getline(ss, taken, '.'))
	{
		vec.push_back(taken);
	}
	return vec.back();
}