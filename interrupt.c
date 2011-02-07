#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
#include "interrupt.h"

static const int SIGNAL_TYPE = SIGALRM;
static void setAlarm();
static void setSet(sigset_t *setp);
int loud = 1;

void interruptsOn()
{
	sigset_t sigset;
	setSet(&sigset);
	int ret = sigprocmask(SIG_UNBLOCK, &sigset, NULL);
	assert(!ret);
}

void interruptsOff()
{
	sigset_t sigset;
	setSet(&sigset);

	int ret = sigprocmask(SIG_BLOCK, &sigset, NULL);
	assert(!ret);
}

static void setSet(sigset_t *setp)
{
	int ret = sigemptyset(setp);
	assert(!ret);
	ret = sigaddset(setp, SIGNAL_TYPE);
	assert(!ret);
	return;
}

void interruptsQuiet()
{
	loud = 0;
}
 
// Should be called when you initialize threads package.
void registerHandler()
{
	// should only register once
	static int hasBeenCalled = 0;
	assert(!hasBeenCalled);
	hasBeenCalled = 1;

	struct sigaction action;
	action.sa_handler = NULL;
	action.sa_sigaction = interruptHandler;

	// SIGNAL_TYPE will be blocked by default while handler invoked. No mask needed.
	int error = sigemptyset(&action.sa_mask);
	assert(!error);

	// use sa_sigaction as handler instead of sa_handler
	action.sa_flags = SA_SIGINFO;
 
	if(sigaction(SIGNAL_TYPE, &action, NULL))
	{
		perror("Setting up signal handler");
		assert(0);
	}

	setAlarm();
}
 
// STUB: once registerhandler is called, this routine is called whenever SIGNAL_TYPE is sent to this process
void interruptHandler(int sig, siginfo_t *sip, void *contextVP)
{
	// NOTUSED
	ucontext_t* context = (ucontext_t*) contextVP;

	// SIGALRM is masked on entry 
	sigset_t mask;
	int ret = sigprocmask(0, NULL, &mask);
	assert(!ret);
	assert(sigismember(&mask, SIGALRM));

	if(loud) printf("Made it to handler with interrupted thread's state at %p\n", context);

	setAlarm();

	// Your code here 

	return;
}
 
// Use the alarm() system call to set an alarm for some time into the future.
// At that time, this process will receive a SIGALRM signal.
static void setAlarm()
{
	static const int SECONDS_TO_NEXT = 1;
	alarm(SECONDS_TO_NEXT);
}

int alarmIsEnabled()
{
	sigset_t mask;
	int ret = sigprocmask(0, NULL, &mask);
	assert(!ret);
	return(!sigismember(&mask, SIGALRM));
}
