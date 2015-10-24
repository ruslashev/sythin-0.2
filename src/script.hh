#ifndef SCRIPT_HH
#define SCRIPT_HH

#include <string>
#include <cstring>
#include <lua.hpp>

class Script
{
	lua_State *L;
	std::string buffer;

	void panic(std::string msg);
public:
	Script();
	~Script();
};

#endif

