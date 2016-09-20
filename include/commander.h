#ifndef REQUEST_CHANNEL
#define REQUEST_CHANNEL
#include "../include/nexus_utility.h"

#include "../include/data_channel.h"
#include "../include/request_channel.h"

typedef struct{void *internal, *external;
               fmq_server_t *server;
               zloop_t *reactor;
               lua_State *lua;
               int internal_port, external_port;
               char identity[18], self_uri[256], services_uri[256];} _control_t;

static void _control_register(_control_t *self, zmsg_t *request, zmsg_t *reponse);

static void _control_discover(_control_t *self, zmsg_t *request, zmsg_t *response);

static void _control_h(zloop_t *loop, zmq_pollitem_t *poller, void *arg);

static void _control_configure(_control_t *self);

int l_test(lua_State *lua);

static const struct luaL_Reg l_commander [] = {
  {"start_nexus", l_start_nexus},
  {NULL, NULL}
};

int luaopen_commander (lua_State *lua);
#endif