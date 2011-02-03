#include <stdio.h>
#include <stdlib.h>
#include "ULT.h"

void error(char* string);
void* mallocSafely(size_t size);

typedef struct threadListNodeStruct
{
	struct threadListNodeStruct*	next;
	TCB*				thread;
	
} threadListNode;

typedef struct threadListStruct
{
	threadListNode *head;
} threadList;
  
threadList* threadListInit(TCB* thread);
void threadListAdd(TCB* thread, threadList* list);
void threadListRemove(TCB* thread, threadList* list);
TCB* threadListFind(Tid id, threadList* list);
