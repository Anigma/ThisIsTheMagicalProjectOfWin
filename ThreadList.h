#ifndef _ThreadList_h_
#define _ThreadList_h_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Thread.h"
#include "utility.h"

//A doubly-linked list of threads
typedef struct ThreadListNode_
{
	struct ThreadListNode_*	next;
	struct ThreadListNode_*	previous;
	Thread*			thread;	
} ThreadListNode;

typedef struct ThreadList_
{
	ThreadListNode* head;
} ThreadList;

//Constructor
ThreadList* ThreadListInit();

//Destructor
void ThreadListFree(ThreadList* list);

//Add to the head of the list
void ThreadListAddToHead(ThreadList* list, Thread* thread);

//Return thread by Tid
Thread* ThreadListFind(ThreadList* list, Tid id);

//Remove and return thread by Tid
Thread* ThreadListRemove(ThreadList* list, Tid id);

//Remove and return the end of the list
Thread* ThreadListRemoveEnd(ThreadList* list);

#endif
