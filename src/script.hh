#ifndef SCRIPT_HH
#define SCRIPT_HH

#include <string>
#include <cstring>
#include <lua.hpp>

class Script
{
	lua_State *L;

	void panic(std::string msg);
public:
	std::string source;

	Script();
	~Script();

	void CopyAndExecute(const char *source);
	double GetValue(double omega, double time);
};

#endif

