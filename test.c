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
	test("created thread has a non NULL context", mythread->context != NULL);
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

//previously doTest2
int main2(int argc, char** argv)
{
	registerHandler();
	// Make sure basic thread stuff still works
	testULT();
	// Test preemptive threads
	testULTpreemptive();
	return 0;
}
