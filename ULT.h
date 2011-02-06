#ifndef _ULT_H_
#define _ULT_H_

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "Thread.h"
#include "ThreadList.h"
#include "utility.h"
#include "interrupt.h"

// Some constraints on behavior
static const int ULT_MAX_THREADS = 1024;

// We must implement these
void ULT_Initialize();
Tid ULT_CreateThread(void (*fn)(void*), void* parg);
Tid ULT_Yield(Tid tid);
Tid ULT_Switch(Thread* target);
Tid ULT_DestroyThread(Tid tid);

#endif
