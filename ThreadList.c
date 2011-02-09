#include "ThreadList.h"

//Constructor
ThreadList* ThreadListInit()
{
	ThreadList* list = (ThreadList*) mallocSafely(sizeof(ThreadList));
	list->next = NULL;
	list->previous = NULL;
	list->thread = NULL;
	return list;
}

//Destructor
void ThreadListFree(ThreadList* list)
{
	assert(list);	

	if(list->next != NULL)
		ThreadListFree(list->next);
	free(list);
}

//Add to the head of the list, return the new list pointer
ThreadList* ThreadListAddToHead(ThreadList* list, Thread* thread)
{
	assert(list);
	assert(thread);

	ThreadList* temp 	= ThreadListInit();
	temp->thread 		= thread;
	temp->next		= list;
	list->previous		= temp;
	return temp;
}

//Return thread by Tid
Thread* ThreadListFind(ThreadList* list, Tid id)
{
	assert(list);
	assert(tidValid(id));

	do
	{
		if(list->thread->id == id) return list->thread;
	}
	while((list = list->next));
	return NULL;
}

//Remove and return thread by Tid
Thread* ThreadListRemove(ThreadList* list, Tid id)
{
	assert(list);
	assert(list->thread);
	assert(tidValid(id));

	ThreadList* node = list;
	do
	{
		if(node->thread->id == id)
		{
			//excise from list
			node->previous->next = node->next;
			node->next->previous = node->previous;

			//return the thread and destroy the node
			Thread* thread = node->thread;
			node->next = NULL;
			ThreadListFree(node);
			return thread;
		}
	}
	while((node = node->next));
	return NULL;
}

//Remove and return the end of the list
Thread* ThreadListRemoveEnd(ThreadList* list)
{
	assert(list);

	ThreadList* node = list;
	while(node->next) node = node->next;
	node->previous->next = NULL;
	Thread* thread = node->thread;
	free(node);
	return thread;	
}

