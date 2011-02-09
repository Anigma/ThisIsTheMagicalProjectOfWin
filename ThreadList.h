#ifndef _ThreadList_h_
#define _ThreadList_h_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Thread.h"
#include "utility.h"

//A doubly-linked list of threads
typedef struct ThreadList_
{
	struct ThreadList_*	next;
	struct ThreadList_*	previous;
	Thread*			thread;	
} ThreadList;

//Constructor
ThreadList* ThreadListInit();

//Destructor
void ThreadListFree(ThreadList* list);

//Add to the head of the list, return the new list pointer
ThreadList* ThreadListAddToHead(ThreadList* list, Thread* thread);

//Return by Tid
Thread* ThreadListFind(ThreadList* list, Tid id);

//Remove and return by Tid
Thread* ThreadListRemove(ThreadList* list, Tid id);

//Remove and return the end of the list
Thread* ThreadListRemoveEnd(ThreadList* list);

#endif
