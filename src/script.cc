#include "script.hh"

Script::Script()
{
	L = luaL_newstate();
	luaL_openlibs(L);
}

void Script::CopyAndExecute(const char *source)
{
	int error = luaL_loadfile(L, source);
	error |= lua_pcall(L, 0, LUA_MULTRET, 0);
	if (error)
		panic("failed to load file");
}

double Script::GetValue(double omega, double time)
{
	// inefficient so far
	lua_getglobal(L, "wave");
	lua_pushnumber(L, omega);
	lua_pushnumber(L, time);
	int error = lua_pcall(L, 2, 1, 0);
	if (error)
		panic("failed to call function wave");
	if (!lua_isnumber(L, -1))
		panic("function wave should return a number");
	double result = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

void Script::panic(std::string msg)
{
	const char *err = lua_tostring(L, -1);
	printf("Error \"%s\": %s\n", msg.c_str(), err);

	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	if (lua_pcall(L, 0, 1, 0) != 0) {
		msg = lua_tostring(L, -1);
		printf("Error in debug.traceback() call: %s\n", err);
	} else
		puts(lua_tostring(L, -1));

	throw msg;
}

Script::~Script()
{
	lua_close(L);
}

