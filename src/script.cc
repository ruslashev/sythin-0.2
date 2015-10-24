#include "script.hh"

Script::Script()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	int error = luaL_loadfile(L, "wave.lua");
	if (error) {
		printf("Couldn't load file: %s\n", lua_tostring(L, -1));
	}

	lua_getglobal(L, "wave");
	lua_pushnumber(L, 100.0); // 1st argument
	lua_pushnumber(L, 150.0); // 2nd argument

	// do the call (2 arguments, 1 result)
	if (lua_pcall(L, 2, 1, 0) != 0)
		printf("error running function: %s\n",
				lua_tostring(L, -1));

	// retrieve result
	if (!lua_isnumber(L, -1))
		printf("function `wave' must return a number\n");
	double result = lua_tonumber(L, -1);
	lua_pop(L, 1); // pop returned value
	printf("%f\n", result);
}

Script::~Script()
{
	lua_close(L);
}

