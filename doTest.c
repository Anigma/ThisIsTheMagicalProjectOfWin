#include <assert.h>
#include <stdio.h>
#include "interrupt.h"
#include "basicThreadTests.h"
#include "ULT.h"
#include "utility.h"
#include "Thread.h"
#include "ThreadList.h"


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

int main(int argc, char **argv)
{
	testThread();
	testThreadList();
	basicThreadTests();
	grandFinale();
	return 0;
}
