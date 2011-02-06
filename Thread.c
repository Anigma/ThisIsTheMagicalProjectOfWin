#include "Thread.h"

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

