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

//change the program counter to point to a stub function that the thread should run.
// malloc() to allocate a new stack, and you will initialize the new stack to include arguments to the stub function.
// change the stack pointer to point to the top of the new stack. (Warning: in x86, stacks grow down!)


// The first thread's context will be slightly different, we'll see how it goes.
ucontext_t* contextInit(void(*fn) (void*), void* parg)
{
	ucontext_t* context = (ucontext_t*) mallocSafely(sizeof(ucontext_t));
	
	context->uc_link = NULL;
	
	//context->sigset_t = ??;

	//Setup the stack, including the SP and args
	(context->uc_stack).ss_sp = calloc(sizeof(int)*(MIN_STACK), 1);
	(context->uc_stack).ss_size = MIN_STACK;
	
	void* ESP = (context->uc_stack).ss_sp + MIN_STACK - 100;
	
	//Set ESP
	(context->uc_mcontext).gregs[REG_ESP] = (int) ESP;
	//Set EIP to *fn 
	(context->uc_mcontext).gregs[REG_EIP] = (int) fn;
	
	//Write *arg 	 to ESP + 8
	//Write *fn  	 to ESP + 4
	//Write 0 (Ret*) to ESP + 0
	
	//context->mcontext_t = ??;
	return context;
}

void ThreadStoreContext(Thread* thread)
{
	//assert that the currently running thread is the thread to which we are storing
	//assert(runningThread->id == thread->id);
	//memory leak
	thread->context = getContext();		
	
	printf("Stored context: \n");
	printUContext(thread->context);
}

void ThreadRun(Thread* thread)
{
printf("setting context\n"); fflush(stdout);
	setcontext(thread->context);
printf("set context\n"); fflush(stdout);

}
