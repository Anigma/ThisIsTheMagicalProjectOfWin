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
	test("thread's context verifies", 1);
	ThreadFree(mythread);
}

void testThreadList()
{
	Tid id;
	Thread* thread;
	Thread* thread2;

	ThreadList* list = ThreadListInit();
	test("creating threadList", list != NULL);
	Tid first = nextTid;

	thread = ThreadListRemove(list, 2);
	test("return null when trying to remove from empty thread", thread == NULL);
	
	thread = ThreadListRemoveEnd(list);
	test("return null when trying to remove from empty thread", thread == NULL);

	thread = ThreadInit(getContext());
	ThreadListAddToHead(list, thread);
		
	thread2 = ThreadListRemoveEnd(list);
	test("remove last element", thread == thread2);
	
	thread = ThreadListRemove(list, 999);
	test("return null when trying to remove from empty thread", thread == NULL);

	first = nextTid;

	int i;
	for(i=0; i<10; i++)
	{
		ThreadListAddToHead(list, ThreadInit(getContext()));
	}

	id = ThreadListRemove(list, first+5)->id;
	test("removed thread from list", id == first+5);
	Thread* none = ThreadListRemove(list, first+5);
	test("don't remove a non-existant Tid from list", none == NULL);
	ThreadListAddToHead(list, ThreadInit(getContext()));
	test("add another node to the the list", 1);
	thread = ThreadListFind(list, first+4);
	test("find a thread from the list, don't remove", thread->id == first+4);
	thread = ThreadListRemoveEnd(list);
	test("remove from end of list", thread->id == first);
	ThreadListFree(list);
	test("freed list", 1);	
}

int main(int argc, char** argv)
{
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
	testULT();
	testULTpreemptive();
	return 0;
}
