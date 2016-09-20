#ifndef DATA_CHANNEL
#define DATA_CHANNEL

#include "nexus_utility.h"

#define UPDATE_CHANNEL "bonsai_update"
#define QUERY_CHANNEL  "bonsai_query"

// -- Begin Data Channel -----------------------------------------------------------------------------------------------
typedef struct{void *update, *control;
               void *query;
               zctx_t *ctx;
               zloop_t *reactor;
               lua_State *lua;} _data_t;

static void _data_update_h(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
  fprintf(stderr, "HA HA HA! Right...data commands. Try again later.\n");
}

static void _data_query_h(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
  fprintf(stderr, "HA HA HA! Right...query commands. Try again later.\n");
}

static void _data_control_h(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
   fprintf(stderr, "HA HA HA! Right...centralized commands. Try again later.\n");
}

int start_data_c(void *args)
{
  zclock_sleep (100*10);
  int rc;
  char uri[256];
  const char *identity, *control_channel_uri;

  char **params = (char **) args;
  _data_t *self;

  identity = params[0];
  control_channel_uri = params[1];

  self = (_data_t *)zmalloc(sizeof(_data_t));

  //Setup sockets, reactors and bind to methods
  {
    printf ("Starting Internal Data Query/Update Channel...\n");
    printf ("\t%s, %s\n", identity, control_channel_uri);
    self->ctx = zctx_new (); assert (self->ctx);

    //-- Control (Passive Consumer) -----
    self->control = zsocket_new (self->ctx, ZMQ_DEALER); assert(self->control);
    zsocket_set_identity(self->control, "data_channel"); //TODO: SEND REGISTER with all services, or not
    assert(zsocket_connect(self->control, control_channel_uri) > -1);

    //-- Update (Active Consumer) -----
    self->update = zsocket_new (self->ctx, ZMQ_DEALER); assert(self->update);
    zsocket_set_identity(self->update, (char*)identity);  //  zsockopt_set_subscribe (self->update, "bonsai"); zsockopt_set_subscribe (self->update, (char*)identity);
    _discover_service(self->control, UPDATE_CHANNEL, uri);
    assert(zsocket_connect(self->update, uri) > -1);

    //-- Query (Service) -----
    self->query =zsocket_new (self->ctx, ZMQ_ROUTER); assert(self->query);
    zsocket_set_identity(self->update, (char*)identity);

    _generate_uri(self->control, uri);        //consider fixed URI
    rc = zsocket_bind(self->query, uri);
      printf("Look at this %s\n", zsocket_last_endpoint(self->query));
    assert(rc > -1);
    //TODO: _register_service();

    //-- Lua storage/logic helper for query/update
    self->lua = luaL_newstate();
    luaL_openlibs(self->lua);
    luaL_loadfile(self->lua, "./helpers.lua");
    lua_pcall(self->lua,0,0,0);
  }

  //Reactor setup and binding
  self->reactor = zloop_new();
  zmq_pollitem_t query_poll   = { self->query, 0, ZMQ_POLLIN, 0 };
  zmq_pollitem_t update_poll  = { self->update, 0, ZMQ_POLLIN, 0 };
  zmq_pollitem_t control_poll = { self->control, 0, ZMQ_POLLIN, 0 };

  //zloop_timer(self->reactor, 100, 1, (void *)_util_pulse, self->update);
  //zloop_timer(self->reactor, 10, 0, (void *)_util_register, self->query);

  assert (zloop_poller (self->reactor, &query_poll,  (void *) _data_query_h, self) == 0);
  assert (zloop_poller (self->reactor, &update_poll,  (void *) _data_update_h, self) == 0);
  assert (zloop_poller (self->reactor, &control_poll,  (void *) _data_control_h, self) == 0);

  printf ("Data Channel Started...\n");
  zloop_start (self->reactor);

  //TODO: Send *dead* signal
  //zloop_poller_end(self->reactor, &control_poll);

  lua_close(self->lua);
  zloop_destroy (&self->reactor); assert (self->reactor == NULL);
  zctx_destroy (&self->ctx);
  free(self);

return 0;
}
// -- End Data Channel -------------------------------------------------------------------------------------------------
#endif