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

// Not sure where this goes, but we'll need to build a context for each thread.
// The first thread's context will be slightly different, we'll see how it goes.
ucontext_t* contextInit()
{
	ucontext_t* context = (ucontext_t*) mallocSafely(sizeof(ucontext_t));
	context->uc_link = NULL;
	//context->sigset_t = ??;
	(context->uc_stack).ss_sp = calloc(sizeof(int)*(MIN_STACK), 1);
	//not using mallocSafely because we need to be able to return ULT_NOMEMORY from ULT_createThread 
	(context->uc_stack).ss_size = MIN_STACK;
	//context->mcontext_t = ??;
	return context;
}

void ThreadStoreContext(Thread* myThread)
{
	//assert that the currently running thread is the thread to which we are storing	
}
