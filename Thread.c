#include "Thread.h"

void ThreadFree(Thread* myThread)
{
	//printf("Freeing thread[%d], context[0x%.8x]\n", myThread->id, (int) myThread->context);
	//free(myThread->context);
	//free(myThread);
}

Thread* ThreadInit(ucontext_t* context)
{
	Thread* temp =(Thread*) mallocSafely(sizeof(Thread));
	temp->context	= context;
	temp->id	= nextTid++;
	return temp;
}

void verifyMcontext(mcontext_t* mcontext)
{
	/* mcontext_t has the following fields:
		gregset_t	gregs;		//array of 19 ints 
		fpregset_t	fpregs;		//a pointer to a struct with a bunch of ulongs
		ulong		oldmask;	//not sure what oldmask is,	getcontext leaves it 0
		ulong		cr2;		//not sure what cr2 is,		getcontext leaves it 0
	*/

	assert(mcontext != NULL);
	assert(sizeof(mcontext_t) == 22*sizeof(int));
	assert(sizeof(gregset_t ) == 19*sizeof(int));
	assert(mcontext->fpregs != NULL);
	assert(sizeof(fpregset_t) ==1*sizeof(int));
}

void verifyStack(stack_t* stack)
{
	/* stack_t has the following fields:
		void*	ss_sp;		//pointer to the stack segment, getcontext leaves it 0
		int	ss_flags;	//not sure what ss_flags is,	getcontext leaves it 0
		size_t	ss_size;	//not sure what ss_size is,	getcontext leaves it 0
	*/
	assert(stack != NULL);
	assert(sizeof(stack_t) == 3*sizeof(int));
	//assert(stack->ss_sp != NULL); //TODO fix
}

void verifyContext(ucontext_t* context)
{
	/* ucontext_t has the following fields:
		ucontext_t*	uc_link;	//we won't use this field, see makecontext
		sigset_t	uc_sigmask;	//signals blocked by thread, see sigprocmask, signal.h
		stack_t		uc_stack;	//stack used by this context, see sigaltstack, bits/sigstack.h
		mcontext_t	uc_mcontext;	//machine state, see ucontext.h
		29 unknown words...
	*/
	assert(context != NULL);
	assert(sizeof(ucontext_t) == 87*sizeof(int));
	assert(context->uc_link == NULL);
	assert(sizeof(sigset_t) == 32*sizeof(int));
	verifyStack(&context->uc_stack);
	verifyMcontext(&context->uc_mcontext);
}

ucontext_t* contextInit(void(*fn) (void*), void* parg)
{
	ucontext_t* context = getContext();
	context->uc_link = NULL;
	verifyContext(context);
	(context->uc_stack).ss_sp = calloc(sizeof(int)*(MIN_STACK), 1);
	(context->uc_stack).ss_size = MIN_STACK;
	int* ESP = (int*) (context->uc_stack).ss_sp;
	ESP += MIN_STACK - 100;
	(context->uc_mcontext).gregs[REG_ESP] = (int) ESP;
	(context->uc_mcontext).gregs[REG_EIP] = (int) &stub;
	*(ESP + 2) = (int) parg;
	*(ESP + 1) = (int) fn;
	*(ESP + 0) = 0;
	return context;
}

void ThreadStoreContext(Thread* thread)
{
	thread->context = getContext();		
}

void ThreadRun(Thread* thread)
{
	setcontext(thread->context);
}
