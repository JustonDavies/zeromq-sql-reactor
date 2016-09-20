#include "nexus_handle.h"
#include "json_example.h"

// -- Begin Control Channel --------------------------------------------------------------------------------------------
int l_test(lua_State *lua){
    //Version check
    fprintf(stdout, "Lua version is:     %s.%s.%s\n", LUA_VERSION_MAJOR,  LUA_VERSION_MINOR,  LUA_VERSION_RELEASE);
    fprintf(stdout, "0MQ version is:     %d.%d.%d\n", ZMQ_VERSION_MAJOR,  ZMQ_VERSION_MINOR, ZMQ_VERSION_PATCH);
    fprintf(stdout, "CZMQ version is:    %d.%d.%d\n", CZMQ_VERSION_MAJOR, CZMQ_VERSION_MINOR, CZMQ_VERSION_PATCH);

    fprintf(stdout, "JSON version is:    %d.%d.%d\n", JSON_C_MAJOR_VERSION, JSON_C_MINOR_VERSION, JSON_C_MICRO_VERSION);

    fprintf(stdout, "Sodium version is:  %s\n", SODIUM_VERSION_STRING);
    fprintf(stdout, "SQLite3 version is: %s\n", SQLITE_VERSION);

//    test_json();

    return 1;
}
// -- End Control Channel ----------------------------------------------------------------------------------------------

// -- Begin Lua Import -------------------------------------------------------------------------------------------------
static const struct luaL_Reg l_nexus_handle [] = {
        {"test", l_test},
        {NULL, NULL}
};

int luaopen_nexus_handle (lua_State *lua)
{
#if LUA_VERSION_NUM >= 502
    lua_newtable(lua);
    luaL_setfuncs(lua, l_nexus_handle, 0);
#else
    luaL_register(lua, "nexus_handle", l_nexus_handle); /* 5.1 */
#endif
    return 1;
}
// -- End Lua Import ---------------------------------------------------------------------------------------------------

