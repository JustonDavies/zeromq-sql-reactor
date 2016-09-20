#include <czmq.h>
#include <stdio.h>
#include <string.h>

typedef struct{void *hash;
               zloop_t *reactor;} _fmn_t;


//add a frame using a key
int fmn_add(_fmn_t *self, char *key, void *value, int duration)
{

  return 0;
}

//used to remove and then destroy a frame
int fmn_remove(_fmn_t *self, char *key, void *item)
{

  return 0;
}

void * fmn_new()
{
  _fmn_t *self;
  self = (_fmn_t *)zmalloc(sizeof(_fmn_t));
  return self;
}

void fmn_destroy(_fmn_t **self)
{
  //destroy everything
}