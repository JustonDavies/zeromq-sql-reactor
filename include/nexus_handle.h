//
// Created by juston.davies on 1/28/16.
//

#ifndef NEXUS_ADAPTER_NEXUS_ADAPTER_H
#define NEXUS_ADAPTER_NEXUS_ADAPTER_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <sqlite3.h>
#include <sodium.h>
#include <czmq.h>

#include <json/json.h>
#include <lua5.2/lua.h>
#include <lua5.2/lualib.h>
#include <lua5.2/lauxlib.h>

int luaopen_nexus_handle (lua_State *lua);

#endif //NEXUS_ADAPTER_NEXUS_ADAPTER_H
