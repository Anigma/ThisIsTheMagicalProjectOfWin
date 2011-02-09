#include "ULT.h"
#include "ThreadList.h"


volatile int initialized = 0;
void ULT_Initialize()
{
	if(initialized)	return;
	else		initialized = 1;

	nextTid = 0;


	//Create thread lists
	alive  = ThreadListInit();
	zombie = ThreadListInit();
	
	//Put current thread in thread list
	runningThread = ThreadInit(getContext());
	ThreadListAddToHead(alive, runningThread);
}

//Creates a thread that will start off running fn, returning the Tid of the new thread
//The created thread is put on a ready queue but does not start execution yet.
//The caller of the function continues to execute after the function returns.
Tid ULT_CreateThread(void(*fn) (void*), void* parg)
{
	ULT_Initialize();

	if(numberOfThreads >= ULT_MAX_THREADS)
	{
		return ULT_NOMORE;
	}
	//create ucontext_t
	ucontext_t* context = contextInit(fn, parg);
	if((context->uc_stack).ss_sp == NULL) //could not allocate stack
	{
		return ULT_NOMEMORY;
	}
	//put in a thread
	Thread* thread = ThreadInit(context);
	//put thread in the readylist
	ThreadListAddToHead(alive, thread);
	return thread->id;
}

//Suspends the caller and activates the thread given by id.
//Caller is put on the ready queue.
Tid ULT_Yield(Tid yieldTo)
{

	ULT_Initialize();

	if(yieldTo == ULT_SELF) //Continue the execution of the  caller
	{
		//Yield to self. This turns the function call into an no-op
		assert(runningThread);
		return ULT_Switch(runningThread);
	}

	if(yieldTo == ULT_ANY) //Invoke any thread on the ready queue except self
	{
		//Take next thread at the head of ready queue and execute it
		Thread* target = ThreadListRemoveEnd(alive);
		if (!target || target->id == runningThread->id)
			return ULT_NONE;
		
		ThreadListAddToHead(alive, target);
		
		return ULT_Switch(target);
	}
	if(yieldTo >= 0)
	{
		//Search ready queue for thread with tid of wantTid and execute it
		Thread* target = ThreadListFind(alive, yieldTo);
		if (!target) {
			printf("Trying to yield to a nonexistant thread\n");
			return ULT_INVALID; //when the requested thread was not on the ready list
		}		
		return ULT_Switch(target);
	}
	else
	{
		//Alert the caller that the identifier tid does not correspond to a valid thread
		return ULT_INVALID;
	}
}

volatile int doneThat;
Tid ULT_Switch(Thread *target)
{
	//printf("Trying to switch to thread with Tid[%d]\n", target->id);

	ULT_Initialize();

       doneThat = 0;
       //save state of current thread to TCB
       //getcontext(&(runningThread->TCB.register)); //returns twice
       //ThreadStoreContext(runningThread);
       getcontext(runningThread->context);	
       if(!doneThat)
       {
	       doneThat = 1;
	       //choose new thread to run
	       //int nextID = schedulingPolicy();
	       //runningThread = &(thread[nextid]);
	       runningThread = target;
	       
	       //copy new thread's TCB to processor
	       //setcontext(&(thread[nextid]->TCB.register)); //never returns
	       ThreadRun(runningThread);
	       assert(0);
	}
	return runningThread->id;
}

//destroys the thread whose identifier is tid
//caller continues to execute and receives the result
Tid ULT_DestroyThread(Tid tid)
{
	ULT_Initialize();

	if(tid == ULT_ANY)
	{
		//destroy any thread except the caller
		//if there are no more other threads available to destroy, return ULT_NONE

	}
	else if(tid == ULT_SELF)
	{
		//destroy the caller, function does not return
		// ZOMBIES!
		//schedule another to run
	}
	else if(tid > 0)
	{
		//destroy the thread with id tid
		//if thread not on list return ULT_INVALID
	}
	return ULT_FAILED;
}
