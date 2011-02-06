#include "ULT.h"
#include "ThreadList.h"

// Thread package state
int numberOfThreads;
ThreadList* list;
ThreadList* zombies;
volatile int initialized = 0;



void ULT_Initialize()
{
	if(initialized) return;

	//Create thread list
	ThreadListInit(list);
	
	//Put current thread in thread list
	Thread* firstThread = ThreadInit(getContext());
	ThreadListAdd(firstThread, list);
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
	return ULT_FAILED;
}
