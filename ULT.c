#include "ULT.h"
#include "threadList.h"

int numberOfThreads;
threadList* list;
volatile int initialized = 0;
void ULT_Initialize()
{
	if(initialized) return;

	//Create thread list
	threadListInit(list);
	
	//Put current thread in thread list
	Thread* firstThread = ThreadInit(getContext());
	threadListAdd(firstThread, list);
}

Tid ULT_CreateThread(void (*fn)(void *), void *parg)
{
	//create ucontext_t
	//put in a thread
	//put thread in list

	return ULT_FAILED;
}

Tid ULT_Yield(Tid wantTid)
{
	if (wantTid == ULT_ANY)
	{
		//Take next thread off head of ready queue and execute
	}
	if (wantTid == ULT_SELF)
	{
		//Search ready queue for thread with tid of wantTid and execute
	}
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
