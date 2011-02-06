#ifndef thread_h_
#define thread_h_

#include <ucontext.h>
#include <stdlib.h>
#include "utility.h"

static const int MIN_STACK = 32768;

// Thread identifiers 
typedef int Tid;
// [0, ULT_MAX_THREADS) specific threads
// 0 the first thread to run before ULT_CreateThread
// [-7, -1] error or control codes
static const Tid ULT_ANY	= -1;
static const Tid ULT_SELF	= -2;
static const Tid ULT_INVALID	= -3;
static const Tid ULT_NONE	= -4;
static const Tid ULT_NOMORE	= -5;
static const Tid ULT_NOMEMORY	= -6;
static const Tid ULT_FAILED 	= -7;
// The tid that the next created thread will be given  
volatile Tid nextTid;
// Verify that a tid is valid
static inline int tidValid(Tid tid)
{
  return (tid >= 0);
}

// Represents a thread control block
typedef struct ThreadStruct
{
	ucontext_t* context;
	Tid id;
} Thread;


Thread* ThreadInit(ucontext_t* context);
void ThreadFree(Thread* thread);


#endif