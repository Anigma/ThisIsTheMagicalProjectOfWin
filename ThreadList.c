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

//Destructors
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
	//ThreadListVerify(list);
	assert(list);
	assert(thread);

	ThreadListNode* old	= list->head;
	ThreadListNode* new 	= ThreadListNodeInit();
	new->thread 		= thread;

	//If there is something in the list, preserve the linkage
	if(old)
	{
		old->previous	= new;
		new->next	= old;
	}
	
	//Put the new node at the head of the list
	list->head = new;
	//ThreadListVerify(list);
}

//Return thread by Tid
Thread* ThreadListFind(const ThreadList* list, Tid id)
{
	//ThreadListVerify(list);
	assert(list);
	assert(tidValid(id));

	ThreadListNode* node = list->head;
	if(node == NULL) return NULL; //list is empty
	do
	{
		assert(node->thread);
		if(node->thread->id == id) return node->thread;
	}
	while((node = node->next));
	//ThreadListVerify(list);
	return NULL;
}

//Remove and return thread by Tid
Thread* ThreadListRemove(ThreadList* list, Tid id)
{
	//ThreadListVerify(list);
	assert(list);
	assert(tidValid(id));

	ThreadListNode* node = list->head;

	//Zero elements
	if(node == NULL) return NULL;

	//One or more elements
	do
	{
		if(node->thread->id == id) //Found it
		{
			if(!node->previous && !node->next) //This was the only element
			{
				list->head = NULL;
				return node->thread;
			}

			if (node->previous)
			{
				if (node->next)
				{
					node->previous->next = node->next;
					node->next->previous = node->previous;
					return node->thread;
				}
				else
				{
					node->previous->next = NULL;
					return node->thread;
				}
			}
			else
			{
				list->head = node->next;
				list->head->previous = NULL;
				return node->thread;
			}

		//ThreadListVerify(list);
		return node->thread;
		}
	}
	while((node = node->next));
	return NULL; //Not found
}

//Remove and return the end of the list
Thread* ThreadListRemoveEnd(ThreadList* list)
{
	//ThreadListVerify(list);
	assert(list);
	ThreadListNode* node = list->head;

	//Zero elements
	if(node == NULL) return NULL;

	//One element
	if(!node->next)
	{
		list->head = NULL;
		return node->thread;
	}

	//Multiple elements
	while(node->next) node = node->next;
	node->previous->next = NULL;
	//ThreadListVerify(list);
	return node->thread;
}

//Print the tids in the list
void ThreadListPrint(const ThreadList* list) 
{
	assert(list);
	ThreadListNode* node = list->head;
	while (node)
	{
		printf("%d ", node->thread->id); fflush(stdout);
		node = node->next;
	}
	printf("\n");
}

//Make sure some basic invariants hold true of a thread list
void ThreadListVerify(const ThreadList* list)
{
	ThreadListNode* node = list->head;
	if(!node) return;
	
	assert(node->previous == NULL);
	assert(node->thread);

	while((node = node->next))
	{
		assert(node->thread);
		assert(node->previous);
	}
}
