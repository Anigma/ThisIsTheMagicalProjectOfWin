#ifndef _threadList_h_
#define _threadList_h_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "utility.h"

typedef struct threadListNodeStruct
{
	struct threadListNodeStruct*	next;
	Thread*				thread;	
} threadListNode;

typedef struct threadListStruct
{
	threadListNode* head;
} threadList;
  
threadList* threadListInit();
void threadListAdd(Thread* thread, threadList* list);
void threadListRemove(Tid id, threadList* list);
Thread* threadListFind(Tid id, threadList* list);

#endif
