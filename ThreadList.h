#ifndef _ThreadList_h_
#define _ThreadList_h_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Thread.h"
#include "utility.h"

//Consider private, for internal use only
typedef struct ThreadListNodeStruct
{
	struct ThreadListNodeStruct*	next;
	Thread*				thread;	
} ThreadListNode;

typedef struct ThreadListStruct
{
	ThreadListNode* head;
} ThreadList;
  
ThreadList* ThreadListInit();
void ThreadListAdd(Thread* thread, ThreadList* list);
void ThreadListRemove(Tid id, ThreadList* list);
Thread* ThreadListFind(Tid id, ThreadList* list);

#endif
