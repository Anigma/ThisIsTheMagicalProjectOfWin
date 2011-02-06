#include <assert.h>
#include <stdio.h>
#include "interrupt.h"
#include "basicThreadTests.h"
#include "ULT.h"
#include "utility.h"
#include "thread.h"


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
	
}


int main(int argc, char **argv)
{
	testThread();
	testThreadList();
	basicThreadTests();
	grandFinale();
	error("Should not have returned from grandFinale()!");
	return 0;
}


