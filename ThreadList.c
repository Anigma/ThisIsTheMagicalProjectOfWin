#include "ThreadList.h"

//Constructors
ThreadListNode* ThreadListNodeInit()
{
	ThreadListNode* node = (ThreadListNode*) mallocSafely(sizeof(ThreadListNode));
	node->next = NULL;
	node->previous = NULL;
	node->thread = NULL;
	return node;
}

ThreadList* ThreadListInit()
{
	ThreadList* list = (ThreadList*) mallocSafely(sizeof(ThreadList));
	list->head = NULL;
	return list;
}

//Destructor
void ThreadListNodeFree(ThreadListNode* node)
{
	assert(node);

	if(node->next != NULL)
		ThreadListNodeFree(node->next);
	free(node);
}

void ThreadListFree(ThreadList* list)
{
	if(list->head) ThreadListNodeFree(list->head);
	free(list);
}

//Add to the head of the list, return the new list pointer
void ThreadListAddToHead(ThreadList* list, Thread* thread)
{
	assert(list);
	assert(thread);

	//Create a new node to add
	ThreadListNode* node 	= ThreadListNodeInit();
	node->thread 		= thread;
	node->previous		= NULL;

	//If there is something in the list, preserve the linkage
	if(list->head)
	{
		list->head->previous	= node;
		node->next		= list->head;
	}
	
	//Put the new node at the head of the list
	list->head = node;
}

//Return thread by Tid
Thread* ThreadListFind(ThreadList* list, Tid id)
{
	assert(list);
	assert(tidValid(id));

	ThreadListNode* node = list->head;
	if(node == NULL) return NULL; //list is empty

	do
	{
		if(node->thread->id == id) return node->thread;
	}
	while((node = node->next));
	return NULL;
}

//Remove and return thread by Tid
Thread* ThreadListRemove(ThreadList* list, Tid id)
{
	assert(list);
	assert(tidValid(id));

	ThreadListNode* node = list->head;
	if(node == NULL) return NULL; //list is empty

	do
	{
		if(node->thread->id == id)
		{
			if (!node->previous /* If found node == head */) {
				list->head = node->next;
				list->head->previous = NULL;
				return node->thread;
			}
		
			//excise from list
			node->previous->next = node->next;
			node->next->previous = node->previous;

			//return the thread and destroy the node
			Thread* thread = node->thread;
			//free(node);
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

	ThreadListNode* node = list->head;
	if(node == NULL) return NULL; //list is empty

	if (!node->next) {
		list->head = NULL;
		return node->thread;
	}

	while(node->next) node = node->next;
	
	node->previous->next = NULL;
	Thread* thread = node->thread;
	
	//free(node);
	return thread;	
}

