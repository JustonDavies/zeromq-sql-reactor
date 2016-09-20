#ifndef REQUEST_CHANNEL
#define REQUEST_CHANNEL

#include "nexus_utility.h"

// -- Begin Request Channel --------------------------------------------------------------------------------------------
typedef struct{void *nexus_queue, *nexus_event, *bonsai_router, *control, *ctx, *lua;} _request_t;

static void _x_in_h(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{}

int start_request_c(void *args){
return 0;
}
// -- End Request Channel ----------------------------------------------------------------------------------------------

#endif