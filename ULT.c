#include <assert.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif /* __USE_GNU */
#include <ucontext.h>

#include "ULT.h"




Tid ULT_CreateThread(void (*fn)(void *), void *parg)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}



Tid ULT_Yield(Tid wantTid)
{
  if (wantTid == ULT_ANY) {
    //Take next thread off head of ready queue and execute
  }

  //Search ready queue for thread with tid of wantTid and execute
  
  return ULT_FAILED;
}

Tid ULT_Switch(ThrdCtlBlk *target) {

  //Switches to the target thread.
  
  return ULT_FAILED;
}


Tid ULT_DestroyThread(Tid tid)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}





