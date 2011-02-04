#include "ULT.h"

void error(char* string)
{
  printf("%s", string);
  exit(-1);
}

void* mallocSafely(size_t size)
{
  if(size==0) error("I refuse to allocate nothing");
  void* temp = malloc(size);
  if(temp==NULL) error("Could not allocate memory!");
  return temp;
}



TCB* threads;
int numberOfThreads;

volatile int initialized = 0;
void ULT_Initialize()
{
	if(initialized) return;

	//Create thread list
	threads = (TCB*) malloc(sizeof(TCB)*ULT_MAX_THREADS);

	//Put current thread in thread list
	//threads[0] = 

}




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
  if (wantTid == ULT_SELF) {
  }

  //Search ready queue for thread with tid of wantTid and execute
  
  return ULT_FAILED;
}

volatile int doneThat;
Tid ULT_Switch(TCB *target)
{
/*
       doneThat = 0;
       //save state of current thread to TCB
       getcontext(T&(runningThread->TCB.register)); //returns twice
       if(!doneThat)
       {
	       doneThat = 1;
	       //choose new thread to run
	       int nextID = schedulingPolicy();
	       runningThread = &(thread[nextid]);
	       //copy new thread's TCB to processor
	       setcontext(&(thread[nextid]->TCB.register)); //never returns
	       assert(false);
	}
*/
	return ULT_FAILED;
}


Tid ULT_DestroyThread(Tid tid)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}





