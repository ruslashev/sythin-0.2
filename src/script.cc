#include "script.hh"

Script::Script()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	int error = luaL_dofile(L, "wave.lua");
	if (error)
		panic("failed to load file");

	lua_getglobal(L, "wave");
	lua_pushnumber(L, 100.0);
	lua_pushnumber(L, 150.0);
	error = lua_pcall(L, 2, 1, 0);
	if (error)
		panic("failed to call function wave");
	if (!lua_isnumber(L, -1))
		panic("function wave should return a number");
	printf("goteem %f\n", (double)lua_tonumber(L, -1));
	lua_pop(L, 1);
}

void Script::panic(std::string msg)
{
	const char *err = lua_tostring(L, -1);
	printf("Error: %s\n", err);

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

