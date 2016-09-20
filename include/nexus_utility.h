#ifndef NEXUS_UTILITY
#define NEXUS_UTILITY

#include <lua5.2/lua.h>
#include <lua5.2/lualib.h>
#include <lua5.2/lauxlib.h>

#include <czmq.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define HELPER_SCOPE "helpers"

#define DISCOVER "discovery"
#define REGISTER "registration"
#define URI_GEN  "generate uri"

// -- Begin Utility Methods --------------------------------------------------------------------------------------------
void _populate_uri(char *container, char* address, int port_number)
{
  if (port_number > 0)
    sprintf(container, "%s:%d\0", address, port_number);
  else
    sprintf(container, "%s:*\0", address);
}

static void _error(lua_State *lua, const char *message_format, ...)
{
  va_list args;
  va_start(args, message_format);
  fprintf(stderr, message_format, args);

  va_end(args);

  //...eh, I'm not so sure about this in the context of ZMQ. I may just want to return.
  //lua_close(lua);
  //exit(EXIT_FAILURE);
}

static void _lua_stack_dump (lua_State *L) {
  int i;
  int top = lua_gettop(L);
  for (i = 1; i <= top; i++) {
    int t = lua_type(L, i);
    switch (t)
    {
      case LUA_TSTRING:  {printf("\"%s\"", lua_tostring(L, i));           break;}
      case LUA_TBOOLEAN: {printf(lua_toboolean(L, i) ? "true" : "false"); break;}
      case LUA_TNUMBER:  {printf("%g", lua_tonumber(L, i));               break;}
      default:           {printf("%s", lua_typename(L, t));               break;}
    }
    fprintf(stderr, " => ");
  }
  fprintf(stderr, "\n");
}

static void _invoke_lua_function(lua_State *lua, const char *function_name, const char *signature, ...)
{
  va_list args;
  int num_args, res_num;
  va_start (args, signature);

  //lua_getglobal(lua, function_name); if (!lua_isfunction(lua, -1)) _error(lua, "Invalid function (%s)\n", function_name);
  lua_getglobal(lua, HELPER_SCOPE); if (!lua_istable(lua, -1)) _error(lua, "Invalid scope (%s)\n", HELPER_SCOPE);
  lua_getfield(lua,-1,function_name); if (!lua_isfunction(lua, -1)) _error(lua, "Invalid function (%s)\n", function_name);
  lua_remove(lua, -2);


  for(num_args = 0; *signature; num_args++)
  {
     luaL_checkstack(lua, 1, "The stack is full! Too many arguments!");
     switch(*signature++)
     {
        case 'd': {lua_pushnumber (lua, va_arg(args, double)); break;}
        case 'i': {lua_pushinteger(lua, va_arg(args, int));    break;}
        case 's': {lua_pushstring (lua, va_arg(args, char *)); break;}
        case 'b': {lua_pushboolean(lua, va_arg(args, int));    break;}

        case '>': {goto endargs;}
        default: {_error(lua, "Invalid option (%c)\n", *(signature-1));}
     }
  }
  endargs:
  res_num = strlen(signature);

  if (lua_pcall(lua, num_args, res_num, 0) != 0)//Stack Change
    _error(lua, "_error calling '%s': %s\n", function_name, lua_tostring(lua, -1));

  res_num = -res_num;
  while(*signature)
  {
    switch (*signature++)
    {
      case 'd':
      {
        if(!lua_isnumber(lua, res_num)) _error(lua, "Wrong result type!\n");
        *va_arg(args, double *) =  lua_tonumber (lua, res_num);
        break;
      }
      case 'i':
      {
        if(!lua_isnumber(lua, res_num)) _error(lua, "Wrong result type!\n");
        *va_arg(args, int *) =  lua_tointeger (lua, res_num);
        break;
      }
      case 's':
      {
        if(!lua_isstring(lua, res_num)) _error(lua, "Wrong result type!\n");
        *va_arg(args, const char **) =  lua_tostring(lua, res_num);
        break;
      }
      case 'b':
      {
        if(!lua_isboolean(lua, res_num)) _error(lua, "Wrong result type!\n");
        *va_arg(args, int *) =  lua_tointeger (lua, res_num);
        break;
      }

      default: {_error(lua, "Invalid option (%c)\n", *(signature-1));}
    }
    res_num++;
  }

  va_end(args); //Returns args as listed
}

static void _discover_service(void *control_socket, const char *service, char *buffer)
{
  assert(control_socket && service);

  zmsg_t *request = zmsg_new (); assert (request);
  char *requester = zsocket_identity(control_socket);

//  zmsg_add (request, zframe_new("control_channel", strlen("control_channel")));
//  zmsg_add (request, zframe_new(requester, strlen(requester)));
  zmsg_addstr (request, "");
  zmsg_addstr (request, DISCOVER);
  zmsg_addstr (request, service);

  assert (zmsg_send (&request, control_socket) == 0);

  zmsg_t *response = zmsg_recv(control_socket);
  if (response)
  {
    char *delimiter = zmsg_popstr(response);
    char *answer = zmsg_popstr(response);

    sprintf(buffer, "%s\0", answer);

    free(answer);
    free(delimiter);
  }
  zmsg_destroy(&response); //causes a corrupt linked list error. No idea why.
}

static void _generate_uri(void *control_socket, char *buffer)
{
  assert(control_socket);
  zmsg_t *request = zmsg_new (); assert (request);
  char *requester = zsocket_identity(control_socket);

  zmsg_addstr (request, "");
  zmsg_addstr (request, URI_GEN);
    //Add control_socket identity

  assert (zmsg_send (&request, control_socket) == 0);

  zmsg_t *response = zmsg_recv(control_socket);
  if (response)
  {
    char *delimiter = zmsg_popstr(response);
    char *answer = zmsg_popstr(response);

    sprintf(buffer, "%s\0", answer);

    free(answer);
    free(delimiter);
  }
  zmsg_destroy(&response); //causes a corrupt linked list error. No idea why.
}

static void _util_pulse(void *pulsing_socket, char *buffer)
{
  assert(pulsing_socket);
  zmsg_t *request = zmsg_new (); assert (request);
  char *requester = zsocket_identity(pulsing_socket);

  zmsg_addstr (request, "");
  zmsg_addstr (request, URI_GEN);

  assert (zmsg_send (&request, pulsing_socket) == 0);

  zmsg_t *response = zmsg_recv(pulsing_socket);
  if (response)
  {
    char *delimiter = zmsg_popstr(response);
    char *answer = zmsg_popstr(response);

    sprintf(buffer, "%s\0", answer);

    free(answer);
    free(delimiter);
  }
  zmsg_destroy(&response); //causes a corrupt linked list error. No idea why.
}

static void _util_register(void *control_socket, char *buffer)
{

}


// -- End Utility Methods ----------------------------------------------------------------------------------------------

#endif