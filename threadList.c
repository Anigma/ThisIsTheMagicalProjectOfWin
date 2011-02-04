#include "threadList.h"

threadList* threadListInit()
{
	threadList* list = (threadList*) mallocSafely(sizeof(threadList));
	list->head = NULL;
	return list;
}

void threadListFree(threadList* list)
{
	assert(list->head == NULL);
	free(list);
}

void threadListAdd(Thread* thread, threadList* list)
{
	//Create the new node
	threadListNode* newNode = (threadListNode*) mallocSafely(sizeof(threadListNode));
	newNode->thread = thread;

	//Add node to the head
	newNode->next = list->head;
	list->head = newNode;
}

Thread* threadListFind(Tid id, threadList* list)
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

			ThreadFree(node->thread);
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
	printf("Testing threadList\n");
	Thread* myThread = ThreadInit(getContext());
	printf("Made a thread\n");
	Tid myTid = myThread->id;
	threadList* list = threadListInit();
	printf("Constructed our list\n");
	threadListAdd(myThread, list);
	printf("Populated our list\n");
	threadListRemove(myTid, list);
	printf("Deleted from list\n");
	threadListFree(list);
	printf("Freed our list\n");
	return 0;
}
