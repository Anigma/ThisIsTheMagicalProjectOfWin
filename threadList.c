#include "threadList.h"

threadList* threadListInit()
{
	threadList* list = (threadList*) mallocSafely(sizeof(threadList));
	list->head = NULL;
	return list;
}

void threadListAdd(TCB* thread, threadList* list)
{
	//Create the new node
	threadListNode* newNode = (threadListNode*) mallocSafely(sizeof(threadListNode));
	newNode->thread = thread;

	//Add node to the head
	newNode->next = list->head;
	list->head = newNode;
}

TCB* threadListFind(Tid id, threadList* list)
{
	threadListNode* node = list->head;
	while(node)
	{
		if(node->thread->id == id) return node->thread;
		node = node->next;
	}
	return NULL;
}

void threadListRemove(Tid id, threadList* list)
{
	threadListNode* node = list->head;
	threadListNode* prev = NULL;
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

			free(node->thread->context);
			free(node->thread);
			free(node);
		}
		//Try the next one
		prev = node;
		node = node->next;
		
	}
	error("Tried to remove a thread from a list that does not contain that thread!");
}


int main()
{
	printf("Testing threadList!\n");
	TCB* myTCB = (TCB*) mallocSafely(sizeof(TCB));
	myTCB->id = 7;
	threadList* list = threadListInit();
	threadListAdd(myTCB, list);
	threadListRemove(7, list);
	free(list);
	return 0;
}
