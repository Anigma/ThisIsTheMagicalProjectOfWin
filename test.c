#include <assert.h>
#include <stdio.h>
#include "interrupt.h"
#include "ULT.h"
#include "utility.h"
#include "Thread.h"
#include "ThreadList.h"
#include "testULT.h"

void verifyMcontext(mcontext_t* mcontext)
{
	/* mcontext_t has the following fields:
		gregset_t	gregs;		//array of 19 ints 
		fpregset_t	fpregs;		//a pointer to a struct with a bunch of ulongs
		ulong		oldmask;	//?
		ulong		cr2;		//?
	*/

	assert(mcontext != NULL);
	assert(sizeof(mcontext_t) == 22*sizeof(int));
	assert(sizeof(gregset_t ) == 19*sizeof(int));
	assert(mcontext->fpregs != NULL);
	assert(sizeof(fpregset_t) ==  1*sizeof(int));
	//TODO not sure what oldmask is,	getcontext leaves it 0
	//TODO not sure what cr2 is,		getcontext leaves it 0
}

void verifyStack(stack_t* stack)
{
	/* stack_t has the following fields:
		void*	ss_sp;		//pointer to the stack segment
		int	ss_flags;	//?
		size_t	ss_size;	//?
	*/
	//If coming from getcontext, everything will be zero!

	assert(stack != NULL);
	assert(sizeof(stack_t) == 3*sizeof(int));
	//assert(stack->ss_sp != NULL);
	//TODO check size and check guard values of stack segment, stack constructor should then also set the guard values
	//TODO not sure what ss_flags is,	getcontext leaves it 0
	//TODO not sure what ss_size is,	getcontext leaves it 0
	//assert(stack->ss_size >= MIN_STACK); // perhaps something like this?
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
	//TODO not sure what sort of signals we need masked, or how they are represented
	verifyStack(&context->uc_stack); //TODO currently fails!!
	verifyMcontext(&context->uc_mcontext);
}

void testThread()
{
	Thread* mythread = ThreadInit(getContext());
	Tid id = mythread->id;
	test("created thread uses the correct id", id == 0);
	verifyContext(mythread->context);
	test("thread's context verifies", 1);
	ThreadFree(mythread);
}

void testThreadList()
{
	ThreadList* list = ThreadListInit();
	test("creating threadList", list != NULL);
}

int main(int argc, char** argv)
{
	//was there a registerHandler() here that accidentally got deleted? -Craig
	testThread();
	testThreadList();
	testULT();

	grandFinale();
	return 0;
}

//previously named doTest2
int main2(int argc, char** argv)
{
	registerHandler();
	// Make sure basic thread stuff still works
	testULT();
	// Test preemptive threads
	testULTpreemptive();
	return 0;
}
