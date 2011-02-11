#include "ULT.h"
#include "ThreadList.h"

//New theads must run stub
//Destroyed implicitly when it returns to stub
void stub(void (*root)(void *), void *arg)
{
	root(arg); // Thread starts by calling root function
	Tid ret = ULT_DestroyThread(ULT_SELF);
	assert(ret == ULT_NONE); // we should only get here if we are the last thread.
	exit(0); // all threads are done, so process should exit 
}

//Initialize if called for the first time. Otherwise, hunt for zombies.
volatile int Maintainenced = 0;
void ULT_Maintainence()
{
	if(Maintainenced) //hunt for zombies
	{
		Thread* thread;
		while((thread = ThreadListRemoveEnd(zombie)))
		{
			//printf("BANG!\n");
			ThreadFree(thread);
		}

		assert(zombie->head == NULL);

		return;
	}
	else //initialize package
	{
		Maintainenced = 1;

		nextTid = 0;

		//Create thread lists
		alive = ThreadListInit();
		zombie = ThreadListInit();
	
		//Put current thread in thread list
		runningThread = ThreadInit(getContext());
		ThreadListAddToHead(alive, runningThread);

		numberOfThreads++;
	}
}

//Creates a thread that will start off running fn, returning the Tid of the new thread
//The created thread is put on a ready queue but does not start execution yet.
//The caller of the function continues to execute after the function returns.
Tid ULT_CreateThread(void(*fn) (void*), void* parg)
{
	ULT_Maintainence();

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

	numberOfThreads++;
	return thread->id;
}

//Suspends the caller and activates the thread given by id.
//Caller is put on the ready queue.
Tid ULT_Yield(Tid yieldTo)
{
	ULT_Maintainence();

	//printf("alive list in yield:");
	//ThreadListPrint(alive);

	assert(alive->head);


	if(yieldTo == ULT_SELF) //Continue the execution of the caller
	{
		//Yield to self. This turns the function call into an no-op
		assert(runningThread);
		return ULT_Switch(runningThread);
	}

	if(yieldTo == ULT_ANY) //Invoke any thread on the ready queue except self
	{
		//Take next thread at the head of ready queue and execute it
		Thread* target = ThreadListRemoveEnd(alive);
		if (!target) return ULT_NONE;
		if (target->id == runningThread->id)
		{
			ThreadListAddToHead(alive, target);
			return ULT_NONE;
		}
		
		ThreadListAddToHead(alive, target);	
		return ULT_Switch(target);
	}
	if(yieldTo >= 0)
	{
		//Search ready queue for thread with tid of wantTid and execute it
		Thread* target = ThreadListFind(alive, yieldTo);
		if (!target) {
			//printf("Trying to yield to a nonexistant thread\n");
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

	ULT_Maintainence();

	doneThat = 0;
	//save state of current thread to TCB
	//getcontext(&(runningThread->TCB.register)); //returns twice
	//ThreadStoreContext(runningThread);
	runningThread->context = getContext();	
	if(!doneThat)
	{
		doneThat = 1;
		//choose new thread to run
		//assert(runningThread != target); // this is odd - Craig
		runningThread = target;
		
		//copy new thread's TCB to processor
		ThreadRun(runningThread); //never returns
		//test("Should never reach this location", 0);
		//assert(0);
	}
	return target->id;
}

//destroys the thread whose identifier is tid
//caller continues to execute and receives the result
Tid ULT_DestroyThread(Tid tid)
{
	ULT_Maintainence();

	if(tid == ULT_ANY)
	{
		//destroy any thread except the caller
		//if there are no more other threads available to destroy, return ULT_NONE
		if (alive->head->next == NULL)
			return ULT_NONE;

		Thread* target;
		do {
			target = ThreadListRemoveEnd(alive);
			if (!target)
				return ULT_NONE;

			ThreadListAddToHead(alive, target);
		} while (target->id == runningThread->id);

		ThreadListRemove(alive, target->id);
		ThreadListAddToHead(zombie, target);
		numberOfThreads--;

		//printf("Thead[%d] zombified\n", target->id);

		return target->id;
	}
	else if(tid == ULT_SELF)
	{
		//destroy the caller, function does not return
		// ZOMBIES!
		//schedule another to run
		ThreadListRemove(alive, runningThread->id);
		ThreadListAddToHead(zombie, runningThread);
		numberOfThreads--;

		//printf("Thead[%d] zombified\n", runningThread->id);
	
		ULT_Yield(ULT_ANY);		
	}
	else if(tid > 0)
	{
		//destroy the thread with id tid
		Thread* target = ThreadListFind(alive, tid);
		
		if (!target)
			return ULT_INVALID;
	
		ThreadListRemove(alive, target->id);
		ThreadListAddToHead(zombie, target);
		numberOfThreads--;

		//printf("Thead[%d] zombified\n", target->id);

		return target->id;
	}
	return ULT_FAILED;
}
