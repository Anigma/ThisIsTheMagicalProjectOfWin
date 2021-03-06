#include "ULT.h"
#include "ThreadList.h"

//New theads must run stub
//Destroyed implicitly when it returns to stub
void stub(void (*root)(void *), void *arg)
{
	root(arg); // Thread starts by calling root function
	//printf("Thread[%d] ending\n", runningThread->id);
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
		while((thread = ThreadListRemoveEnd(zombie))) ThreadFree(thread);
		assert(zombie->head == NULL);
		return;
	}
	else //initialize package
	{
		Maintainenced = 1;
		nextTid = 0;
		alive	= ThreadListInit();
		zombie	= ThreadListInit();
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
	if(numberOfThreads >= ULT_MAX_THREADS) return ULT_NOMORE;
	ucontext_t* context = contextInit(fn, parg);
	if((context->uc_stack).ss_sp == NULL) return ULT_NOMEMORY;
	Thread* thread = ThreadInit(context);
	ThreadListAddToHead(alive, thread);
	numberOfThreads++;
	return thread->id;
}

//Suspends the caller and activates the thread given by id.
//Caller is put on the ready queue.
Tid ULT_Yield(Tid yieldTo)
{
	ULT_Maintainence();
	ThreadListVerify(alive);
	//assert(alive->head);
	Tid ret;

	if(yieldTo == ULT_SELF) //Continue the execution of the	caller
	{
		//Yield to self. This turns the function call into an no-op
		assert(runningThread);
		ret = ULT_Switch(runningThread);
	}

	else if(yieldTo == ULT_ANY) //Invoke any thread on the ready queue except self
	{
		//Take next thread at the end of ready queue and execute it
		Thread* target = ThreadListRemoveEnd(alive);
		if (target == NULL) ret = ULT_NONE;
		else if (target->id == runningThread->id) //next thread to run is self, so don't do anything. Bug?
		{
			ThreadListAddToHead(alive, target);
			ret = ULT_NONE;
		}
		else
		{
			ThreadListAddToHead(alive, target);
			ret = ULT_Switch(target);
		}
	}
	else if(yieldTo >= 0) //specific id
	{
		Thread* target = ThreadListFind(alive, yieldTo);
		if (!target)	ret = ULT_INVALID; //not found	
		else		ret = ULT_Switch(target);
	}
	else
	{
		ret = ULT_INVALID;
	}
	ThreadListVerify(alive);
	return ret;
}

volatile int doneThat;
Tid ULT_Switch(Thread *target)
{
	//printf("Trying to switch to thread with Tid[%d, context:[0x%.8x]\n", target->id, (int) target->context);
	ULT_Maintainence();
	doneThat = 0;
	assert(runningThread->context);
	getcontext(runningThread->context);
	if(!doneThat)
	{
		doneThat = 1;
		runningThread = target;
		//printf("Switched to thread with Tid[%d]\n", target->id);
		ThreadRun(runningThread);
		assert(0);
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
		if (alive->head->next == NULL)	return ULT_NONE;
		if (alive->head == NULL)	return ULT_NONE;
		Thread* target;
		do
		{
			target = ThreadListRemoveEnd(alive);
			if (!target) return ULT_NONE;
			ThreadListAddToHead(alive, target);
		} while(target->id == runningThread->id);

		ThreadListRemove(alive, target->id);
		ThreadListAddToHead(zombie, target);
		numberOfThreads--;
		//printf("Thead[%d] zombified\n", target->id);
		return target->id;
	}
	else if(tid == ULT_SELF)
	{
		//destroy the caller, function does not return
		//schedule another to run
		ThreadListRemove(alive, runningThread->id);
		ThreadListAddToHead(zombie, runningThread);
		numberOfThreads--;
		//printf("Thead[%d] zombified\n", runningThread->id);
		ULT_Yield(ULT_ANY);
		return ULT_NONE;
		//return runningThread->id;
	}
	else if(tid > 0)
	{
		//destroy the thread with id tid
		Thread* target = ThreadListFind(alive, tid);
		if (!target) return ULT_INVALID;
		ThreadListRemove(alive, target->id);
		ThreadListAddToHead(zombie, target);
		numberOfThreads--;
		//printf("Thead[%d] zombified\n", target->id);
		return target->id;
	}
	else return ULT_FAILED;
}
