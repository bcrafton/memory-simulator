
#include "priority_list.h"

Prioritylist* priority_list_constructor( int (*compare_function)(void*, void*) )
{
	PriorityList* priority_list = (Prioritylist*) malloc(sizeof(Prioritylist));
	priority_list->list = list_constructor();
	priority_list->compare_function = compare_function;
	return priority_list;
}

PriorityListNode* priority_list_node_constructor(KEY_TYPE key, VALUE_TYPE value)
{
	PriorityListNode* new_node = (PriorityListNode*) malloc (sizeof(PriorityListNode));
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	new_node->prev = NULL;
}

void priority_list_push(KEY_TYPE key, VALUE_TYPE value, Prioritylist *list)
{
	PriorityListNode* new_node = priority_list_node_constructor(key, value);
	if(list->head == NULL && list->tail == NULL)
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		PriorityListNode* ptr;
		for(ptr=list->head; ptr=ptr->next; ptr != NULL)
		{
			if( (*(list->compare_function))(new_node->key, ptr->key) < 0 )
			{
				
			}
		}
	}

}

VALUE_TYPE priority_list_pop(Prioritylist *list)
{

}

VALUE_TYPE priority_list_front(Prioritylist *list)
{

}

int priority_list_empty(Prioritylist *list)
{

}