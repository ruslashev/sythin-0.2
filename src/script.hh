#ifndef SCRIPT_HH
#define SCRIPT_HH

#include <string>
#include <lua.hpp>

class Script
{
	lua_State *L;
	std::string buffer;
public:
	Script();
	~Script();
};

#endif

