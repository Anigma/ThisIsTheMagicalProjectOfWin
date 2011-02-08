#include "ULT.h"
#include "ThreadList.h"

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

//Suspends the caller and activates the thread given by id.
//Caller is put on the ready queue.
Tid ULT_Yield(Tid yieldTo)
{
	if(yieldTo == ULT_SELF) //Continue the execution of the  caller
	{
		//Yield to self
		//This turns the function call into an no-op
	}

	if(yieldTo == ULT_ANY) //Invoke any thread on the ready queue
	{
		//Take next thread at the head of ready queue and execute it
		return 0; //return the Tid of the thread yielded to
		return ULT_NONE; //when no threads on ready list
	}
	if(yieldTo > 0)
	{
		//Search ready queue for thread with tid of wantTid and execute it
		return yieldTo;
		return ULT_INVALID; //when the requested thread was not on the ready list
	}
	else
	{
		//Alert the caller that the identifier tid does not correspond to a valid thread
		return ULT_FAILED;
	}
}

volatile int doneThat;
Tid ULT_Switch(Thread *target)
{

       doneThat = 0;
       //save state of current thread to TCB
       //getcontext(&(runningThread->TCB.register)); //returns twice
       if(!doneThat)
       {
	       doneThat = 1;
	       //choose new thread to run
	       //int nextID = schedulingPolicy();
	       //runningThread = &(thread[nextid]);
	       //copy new thread's TCB to processor
	       //setcontext(&(thread[nextid]->TCB.register)); //never returns
	       assert(-1);
	}
	return ULT_FAILED;
}

Tid ULT_DestroyThread(Tid tid)
{
	return ULT_FAILED;
}
