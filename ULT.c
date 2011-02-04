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

ucontext_t* getContext()
{
	ucontext_t* temp = (ucontext_t*) mallocSafely(sizeof(ucontext_t));
	int err = getcontext(temp);
	if(err) error("Could not get context!");
	return temp;
}


void ThreadFree(Thread* myThread)
{
	free(myThread->context);
	free(myThread);
}

Thread* ThreadInit(ucontext_t* context)
{
	Thread* temp =  (Thread*) mallocSafely(sizeof(Thread));
	temp->context	= context;
	temp->id	= nextTid++;
	return temp;
}



int numberOfThreads;

volatile int initialized = 0;
void ULT_Initialize()
{
	if(initialized) return;

	//Create thread list

	//Put current thread in thread list
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
Tid ULT_Switch(Thread *target)
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





