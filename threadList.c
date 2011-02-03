#include <stdio.h>
#include <stdlib.h>
#include "ULT.h"
#include "threadList.h"

void error(char* string)
{
  printf("%s", string);
  exit(-1);
}

void* mallocSafely(size_t size)
{
  if(size==0) error("I refuse to allocate nothing");
  void* temp = malloc(size);
  if(temp==NULL) error("Could not allocate memory!");
  return temp;
}


threadList* threadListInit(TCB* thread)
{
  if(thread==NULL) error("Trying to add null thread!");
	
  threadList* list = (threadList*) mallocSafely(sizeof(threadList));
	
  list->head = NULL;
	
  return list;
}


void threadListAdd(TCB* thread, threadList* list)
{
  threadListNode *head = list->head;

  if (head == NULL) {
    list->head = (threadListNode *) mallocSafely(sizeof(threadListNode));
    list->head->thread = thread;
    list->head->next = NULL;
  }

  head = (threadListNode *) mallocSafely(sizeof(threadListNode));
  head->next = list->head;

  list->head = head;
}


void threadListRemove(TCB* thread, threadList* list)
{
  /*threadList *curr;

  if (curr->next == NULL) {
    if (list->thread == thread)
      free(thread);
  }

  while (curr->next->next != NULL)

    while(list->thread != thread)
      {
	if(list->next) list = list->next;
	else error("Could not find thread in list!");
      }
	
  */
}


TCB* threadListFind(Tid id, threadList* list)
{
  return NULL;
}


int main()
{
  printf("Testing threadList!\n");
  TCB* myTCB = (TCB *) mallocSafely(sizeof(TCB));
  threadList* list = threadListInit(myTCB);
  free(list);

  return 0;
}
