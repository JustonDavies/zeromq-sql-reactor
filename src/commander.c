#include "../include/commander.h"

// -- Begin Control Channel --------------------------------------------------------------------------------------------

static void _control_uri_gen(_control_t *self, zmsg_t *request, zmsg_t *response)
{
//Do whatever you got to do to designate a new uri, for proc or unix file style sockets this would get worse
   char uri[256];
   _populate_uri(uri, self->services_uri, false);
   zmsg_addstr (response, uri);
}

static void _control_register(_control_t *self, zmsg_t *request, zmsg_t *response)
{
   printf("Looks like you intend to register service: %s\n", zmsg_popstr(request));
   //free the string
}

static void _control_discover(_control_t *self, zmsg_t *request, zmsg_t *response)
{
  char *requested_channel;//, uri[256];

  requested_channel = zmsg_popstr(request);

  //being hard coded rubbish, rewrite later for actual discovery
  lua_getfield(self->lua, LUA_GLOBALSINDEX, "env"); assert(lua_istable(self->lua, -1));
  lua_getfield(self->lua, -1, "services");          assert(lua_istable(self->lua, -1));
  lua_getfield(self->lua, -1, "store");             assert(lua_istable(self->lua, -1));

  lua_getfield(self->lua, -1, requested_channel);
    assert(lua_isstring(self->lua, -1));
    zmsg_addstr (response, lua_tostring(self->lua, -1));
    lua_remove(self->lua, -1);

  lua_remove(self->lua, -1); //store
  lua_remove(self->lua, -1); //services
  lua_remove(self->lua, -1); //env

  free(requested_channel);
}

static void _internal_h(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
  zframe_t *current_frame;
  zmsg_t *response, *request;
  char *request_type;

  _control_t *self = (_control_t *) arg;

  request = zmsg_recv (self->internal);

  if (request) {
    response = zmsg_new(); assert (response);

    //Remove address envelope and add it to return
    int envelope = true;
    while(envelope)
    {
      current_frame = zmsg_pop(request);
      zmsg_add(response, current_frame);
      if (zframe_streq(current_frame, "")) envelope = false;
    }

    //Process request
    request_type = zmsg_popstr(request);

    if(strcmp(DISCOVER, request_type) == 0)
      _control_discover(self, request, response);
    else if(strcmp(REGISTER, request_type) == 0)
      _control_register(self, request, response);
    else if(strcmp(URI_GEN, request_type) ==0)
      _control_uri_gen(self, request, response);
    else
     {
       fprintf(stderr, "Invalid control request type: %d", request_type);
       zmsg_addstr (response, "Invalid control request");
     }

     assert (zmsg_send (&response, self->internal) == 0);
   }
   free(request_type);
   //zframe_destroy(&current_frame);  Destroyed by msg send
   //zmsg_destroy(&response); Destroyed by send
   zmsg_destroy(&request);
}

static void _control_configure(_control_t *self)
{
  lua_getfield(self->lua, LUA_GLOBALSINDEX, "env"); assert(lua_istable(self->lua, -1));
  lua_getfield(self->lua, -1, "services");          assert(lua_istable(self->lua, -1));

  lua_getfield(self->lua, -1, "services_uri");
    assert(lua_isstring(self->lua, -1));
    strcpy(self->services_uri, lua_tostring(self->lua, -1));// self->services_uri);
    lua_remove(self->lua, -1);

  lua_getfield(self->lua,-1, "identity");
    assert(lua_isstring(self->lua, -1));
    strcpy(self->identity, lua_tostring(self->lua, -1));
    lua_remove(self->lua, -1);
    assert(strlen(self->identity) == 17);

  lua_remove(self->lua, -1);
  lua_remove(self->lua, -1);
  fprintf(stderr, "services_uri => %s\nidentity => %s\n", self->services_uri, self->identity);
}

int l_start_nexus(lua_State *lua){
  char uri[256];
  _control_t *self;

  self = (_control_t *)zmalloc(sizeof(_control_t));
  self->lua = lua;

  //Version check
  int major, minor, patch;
  zmq_version (&major, &minor, &patch);
  printf ("0MQ version is %d.%d.%d\n", major, minor, patch);
  fprintf(stderr, "CZMQ version is %d.%d.%d\n", CZMQ_VERSION_MAJOR, CZMQ_VERSION_MINOR, CZMQ_VERSION_PATCH);

  //Get identity from freeswitch/lua
  _control_configure(self);

  //Setup context, socket and reactor
  self->ctx = zctx_new (); assert(self->ctx);
  self->internal = zsocket_new (self->ctx, ZMQ_ROUTER); assert(self->internal);
  zsocket_set_identity(self->internal, "control_channel");

  _populate_uri(uri, self->services_uri, false);
  self->internal_port = zsocket_bind(self->internal, uri);
  printf("Look at this %s\n", zsocket_last_endpoint(self->internal));
  assert(self->internal_port > -1);
  _populate_uri(self->self_uri, self->services_uri, self->internal_port);

  self->reactor = zloop_new ();
  //zloop_set_verbose (self->reactor, true);
  zmq_pollitem_t internal = {self->internal, 0, ZMQ_POLLIN, 0 };
  zloop_poller(self->reactor, &internal, (void *)_internal_h, self);

  //TODO: Discover nexus, use external

  //Start the channel threads
  assert(zthread_new ((void*)start_data_c,(const char*[]){(const char*)self->identity, (const char*)self->self_uri})== 0);
  //assert(zthread_new ((void*)start_request_c,(const char*[]){(const char*)self->identity, (const char*)uri})== 0);

  //Start the reactor
  fprintf(stderr, "Commander at the helm...\n");
  zloop_start (self->reactor);

//  while(true){
    zclock_sleep (100);
    if (zctx_interrupted){
      printf ("Commander interrupted, sending kill signal...\n");
//      zmsg_t *msg = zmsg_new (); assert (msg);
//      zmsg_addstr(msg, "signal");
//      zmsg_addstr(msg, "");
//      zmsg_addstr(msg, "kill");
//      assert (zmsg_send(&msg, self->internal) == 0);

      //Do stuff, wait for shutdown signals maybe then...
      zclock_sleep (100*10); //This is pretty lame, but it prevents memory leaks
    }
  //}

  fprintf(stderr, "Shutting down...\n");
  zloop_destroy (&self->reactor);
  zctx_destroy (&self->ctx);
  free(self);

  return 1;
}
// -- End Control Channel ----------------------------------------------------------------------------------------------

// -- Begin Lua Import -------------------------------------------------------------------------------------------------
int luaopen_commander (lua_State *lua)
{
  luaL_register(lua, "commander", l_commander);
  return 1;
}
// -- End Lua Import ---------------------------------------------------------------------------------------------------