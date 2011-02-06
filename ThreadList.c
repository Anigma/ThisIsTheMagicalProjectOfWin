#include "ThreadList.h"

ThreadList* ThreadListInit()
{
	ThreadList* list = (ThreadList*) mallocSafely(sizeof(ThreadList));
	list->head = NULL;
	return list;
}

void ThreadListFree(ThreadList* list)
{
	assert(list->head == NULL);
	free(list);
}

void ThreadListAdd(Thread* thread, ThreadList* list)
{
	//Create the new node
	ThreadListNode* newNode = (ThreadListNode*) mallocSafely(sizeof(ThreadListNode));
	newNode->thread = thread;

	//Add node to the head
	newNode->next = list->head;
	list->head = newNode;
}

Thread* ThreadListFind(Tid id, ThreadList* list)
{
	ThreadListNode* node = list->head;
	while(node)
	{
		if(node->thread->id == id) return node->thread;
		node = node->next;
	}
	return NULL;
}

void ThreadListRemove(Tid id, ThreadList* list)
{
	ThreadListNode* node = list->head;
	ThreadListNode* prev = NULL;
	while(node)
	{
		if(node->thread == NULL) error("Node is degenerate!");
		if(node->thread->id == id)
		{	
			//This one must fix the head
			if(prev == NULL)
			{
				list->head = node->next;
			}

			//Past the head
			else
			{
				prev->next = node->next;
			}

			ThreadFree(node->thread);
			free(node);
		}
		//Try the next one
		prev = node;
		node = node->next;
		
	}
	error("Tried to remove a thread from a list that does not contain that thread!");
}
