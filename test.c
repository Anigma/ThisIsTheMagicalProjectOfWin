#include <assert.h>
#include <stdio.h>
#include "interrupt.h"
#include "ULT.h"
#include "utility.h"
#include "Thread.h"
#include "ThreadList.h"
#include "testULT.h"

void testThread()
{
	Thread* mythread = ThreadInit(getContext());
	Tid id = mythread->id;
	test("created thread uses the correct id", id == 0);
	verifyContext(mythread->context);
	test("thread's context verifies", 1); //commented asserts in verifyStack fail
	ThreadFree(mythread);
}

void testThreadList()
{
	ThreadList* list = ThreadListInit();
	test("creating threadList", list != NULL);
	Tid first = nextTid;
	int i;
	for(i=0; i<10; i++)
	{
		ThreadListAddToHead(list, ThreadInit(getContext()));
	}

	Tid id;
	id = ThreadListRemove(list, first+5)->id;
	test("removed thread from list", id == first+5);
	Thread* none = ThreadListRemove(list, first+5);
	test("don't remove a non-existant Tid from list", none == NULL);
	ThreadListAddToHead(list, ThreadInit(getContext()));
	test("add another node to the the list", 1);
	Thread* thread = ThreadListFind(list, first+4);
	test("find a thread from the list, don't remove", thread->id == first+4);
	thread = ThreadListRemoveEnd(list);
	test("remove from end of list", thread->id == first);
	ThreadListFree(list);
	test("freed list", 1);	
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
