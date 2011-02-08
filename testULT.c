#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h> // Using posix mutex for some tests of phases 1 and 2
#include "testULT.h"
#include "interrupt.h"
#include "ULT.h"
#include "utility.h"

static const int verbose = 0;
static const int vverbose = 0;

static void hello(char *msg);
static int fact(int n);
static void suicide();
static void finale();
static int setFlag(int val);
static void startAlarmHelper();
static void doPotato(int mySlot);
static int tryMovePotato(int mySlot);
static void spin(int secs);

void testULT()
{
	Tid ret;
	Tid ret2;
	// Initial thread yields
	ret = ULT_Yield(ULT_SELF);
	test("Initial thread returns from Yield(SELF)", tidValid(ret));
	ret = ULT_Yield(0);
	test("Initial thread returns from Yield(0)", tidValid(ret));
	ret = ULT_Yield(ULT_ANY);
	test("Initial thread returns from Yield(ANY)",ret == ULT_NONE);
	ret = ULT_Yield(0xDEADBEEF);
	test("Initial thread returns from Yield(INVALID)", ret == ULT_INVALID);
	ret = ULT_Yield(16);
	test("Initial thread returns from Yield(INVALID2)", ret == ULT_INVALID);
	// Create a thread
	ret = ULT_CreateThread((void (*)(void *))hello, "World");
	test("Creates a basic hello world thread", tidValid(ret));
	ret2 = ULT_Yield(ret);
	test("Yielding to this thread returns the correct Tid", ret2 == ret);
	// Create 10 threads
	static const int NTHREAD = 10;
	Tid children[NTHREAD];
	char msg[NTHREAD][1024];
	int childrenOkay = 1;
	int i;
	for(i = 0; i < NTHREAD; i++)
	{
		int err = snprintf(msg[i], 1023, "Hello from thread %d\n", i);
		assert(err > 0); 
		children[i] = ULT_CreateThread((void (*)(void *))hello, msg[i]);
		childrenOkay &= tidValid(children[i]);
	}
	test("Creates 10 children", childrenOkay);
	childrenOkay = 1;
	for(i = 0; i < NTHREAD; i++)
	{
		ret = ULT_Yield(children[i]);
		childrenOkay &= (ret == children[i]);
	}
	test("Yields to 10 children", childrenOkay);

	// Destroy 11 threads we just created
	ret = ULT_DestroyThread(ret2);
	test("Destroys hello world thread", ret == ret2);
	childrenOkay = 1;
	for(i = 0; i < NTHREAD; i++)
	{
		ret = ULT_DestroyThread(children[i]);
		childrenOkay &= (ret == children[i]);
	}
	test("Destroys 10 children", childrenOkay);

	// Create maxthreads-1 threads
	childrenOkay = 1;
	for(i = 0; i < ULT_MAX_THREADS-1; i++)
	{
		ret = ULT_CreateThread((void (*)(void *))fact, (void *)10);
		childrenOkay &= (tidValid(ret));
	}
	test("Creating all available threads", childrenOkay);
	// Now we're out of threads. Next create should fail.
	ret = ULT_CreateThread((void (*)(void *))fact, (void *)10);
	test("Create fails when out of threads", ret == ULT_NOMORE);
	// Now let them all run.
	for(i = 0; i < ULT_MAX_THREADS; i++)
	{
		ret = ULT_Yield(i);
		if(i == 0)
		{ 
			// Guaranteed that first yield will find someone.
			// Later ones may or may not depending on who stub schedules on exit.
			test("Guaranteed that first yield will find someone" ,tidValid(ret));
		}
	}
	// They should have cleaned themselves up when they finished running.
	// Create maxthreads-1 threads.
	childrenOkay = 1;
	for(i = 0; i < ULT_MAX_THREADS-1; i++)
	{
		ret = ULT_CreateThread((void (*)(void *))fact, (void *)10);
		childrenOkay &= (tidValid(ret));
	}
	test("Creates all available threads again", childrenOkay);
	// Now destroy some explicitly and let the others run
	childrenOkay = 1;
	for(i = 0; i < ULT_MAX_THREADS; i+=2)
	{
		ret = ULT_DestroyThread(ULT_ANY);
		childrenOkay &= (tidValid(ret));
	}
	test("Destroys every other thread", childrenOkay);
	// Let the rest run to completion
	for(i = 0; i < ULT_MAX_THREADS; i++)
	{
		ret = ULT_Yield(i);
	}
	ret = ULT_DestroyThread(ULT_ANY);
	test("Destroy won't destroy the last thread", ret == ULT_NONE);
	ret = ULT_DestroyThread(42);
	test("Destroy won't destroy an invalid thread", ret == ULT_INVALID);
	ret = ULT_DestroyThread(-42);
	test("Destroy won't destroy an invalid thread", ret == ULT_INVALID);
	ret = ULT_DestroyThread(ULT_MAX_THREADS + 1000);
	test("Destroy won't destroy an invalid thread", ret == ULT_INVALID);
	// Create a tread that destroys itself. 
	// Control should come back here after that thread runs.
	int flag = setFlag(0);
	ret = ULT_CreateThread((void (*)(void *))suicide, NULL);
	test("Control returns validly after running a suicide thread", tidValid(ret));
	ret = ULT_Yield(ret);
	test("Yields to self successfully", tidValid(ret));
	flag = setFlag(0);
	test("Other thread ran", flag == 1);
	ret = ULT_Yield(ret);
	test("That thread is gone now", ret == ULT_INVALID);
}

void grandFinale()
{
	int ret;
	//printf("For my grand finale, I will destroy myself\n");
	//printf("while my talented assistant prints Done.\n");
	ret = ULT_CreateThread((void (*)(void *))finale, NULL);
	test("Creates a finale thread", tidValid(ret));
	ULT_DestroyThread(ULT_SELF);
	test("Grand finale should never reach this line!!", 0);
}

static void hello(char* msg)
{
	Tid ret;
	printf("Made it to hello() in called thread\n");
	printf("Message: %s\n", msg);
	ret = ULT_Yield(ULT_SELF);
	assert(tidValid(ret));
	printf("Thread returns from first yield\n");
	ret = ULT_Yield(ULT_SELF);
	assert(tidValid(ret));
	printf("Thread returns from second yield\n");
	while(1)
	{
		ULT_Yield(ULT_ANY);
	}
}

static int fact(int n)
{
	if(n == 1) return 1;
	return n*fact(n-1);
}

static void suicide()
{
	int ret = setFlag(1);
	assert(ret == 0);
	ULT_DestroyThread(ULT_SELF);
	test("Suicide should never reache this line!!", 0);
}

//  Using posix mutex for some tests of phases 1 and 2
static pthread_mutex_t posix_mutex = PTHREAD_MUTEX_INITIALIZER;
static int flagVal;

static int setFlag(int val)
{
	int ret;
	int err = pthread_mutex_lock(&posix_mutex);
	assert(!err);
	ret = flagVal;
	flagVal = val;
	err = pthread_mutex_unlock(&posix_mutex);
	assert(!err);
	return ret;
}

static void finale()
{
	int ret = ULT_Yield(ULT_ANY);
	test("Finale is the last remaining thread",ret == ULT_NONE);
	ret = ULT_Yield(ULT_ANY);
	test("Finale is the last remaining thread, really",ret == ULT_NONE);
	// Stub should exit cleanly if there are no threads left to run.
	return; 
}

#define NPOTATO  10
static int potato[NPOTATO];
static Tid potatoTids[NPOTATO];
static const int tPotato = 30;

void testULTpreemptive()
{
	static const int duration = 10;
	int ret;
	startAlarmHelper();
	spin(2);
	interruptsQuiet();
	potato[0] = 1;
	int i;
	for(i = 1; i < NPOTATO; i++)
	{
		potato[i] = 0;
	}
	printf("Running hot potato test. This will take %d seconds\n", duration);
	for(i = 0; i < NPOTATO; i++)
	{
		potatoTids[i] = ULT_CreateThread((void (*)(void *))doPotato, (void *)i);
		assert(tidValid(potatoTids[i]));
	}
	spin(duration);
	printf("Hot potato done. Cleaning up\n");
	for(i = 0; i < NPOTATO; i++)
	{
		ret = ULT_DestroyThread(ULT_ANY);
		assert(tidValid(ret));
	}  
	printf("Done.\n");
}

static void spin(int secs)
{
	struct timeval start, end;
	int ret = gettimeofday(&start, NULL);
	assert(!ret);
	while(1)
	{
		ret = gettimeofday(&end, NULL);
		if(end.tv_sec - start.tv_sec >= secs) break;
	}
}

static void startAlarmHelper()
{
	int ret;
	char command[1024];

	pid_t myPid = getpid();
	snprintf(command, 1024, "./alarmHelper %d &\n", myPid);
	ret = system(command);
	printf("Started alarmHelper to speed up ALARM interupts\n");
	return;
}


static void doPotato(int mySlot)
{
	int ret;
	int moved;
	int passes = 0;

	printf("Made it to doPotato %d\n", mySlot);
	while(1)
	{
		assert(alarmIsEnabled());
		moved = tryMovePotato(mySlot);
		if(moved)
		{
			passes++;
			printf("%d passes potato for %d-st/nd/rd time \n", mySlot, passes);
		}

		// Add some yields by some threads to scramble the list
		if(mySlot > 4)
		{
			int i;
			for(i = 0; i < mySlot - 4; i++)
			{
				ret = ULT_Yield(ULT_ANY);
				assert(tidValid(ret));
			}
		}
	}
}

static int tryMovePotato(int mySlot)
{
	int ret = 0;
	int err = pthread_mutex_lock(&posix_mutex);
	assert(!err);
	if(potato[mySlot])
	{
		ret = 1;
		potato[mySlot] = 0;
		potato[(mySlot + 1) % NPOTATO] = 1;
	}
	err = pthread_mutex_unlock(&posix_mutex);
	assert(!err);
	return ret;
}
