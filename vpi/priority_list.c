
#include "priority_list.h"

static int priority_list_size(Prioritylist *list);

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

// I am skeptical of the correctness of this function
void priority_list_push(KEY_TYPE key, VALUE_TYPE value, Prioritylist *list)
{
	list->size++;
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
            // 3 cases, insert head, insert tail, insert middle
			if( (*(list->compare_function))(new_node->key, ptr->key) < 0 )
			{
				if(ptr->prev == NULL)
                {
                    ptr->prev = new_node;
                    new_node->next = ptr;
                }
                else
                {
                	ptr->prev->next = new_node;
                	new_node->prev = ptr->prev;
                	new_node->next = ptr;
                	ptr->prev = new_node;
                }
                break;
			}
			else if(ptr->next == NULL)
			{
				ptr->next = new_node;
				new_node->prev = ptr;
			}
		}
	}

}

VALUE_TYPE priority_list_pop(Prioritylist *list)
{
	assert(priority_list_size(list) == list->size);
	assert(list->size != 0);

	list->size--;
	VALUE_TYPE ret = list->head->value;
	list->head = list->head->next;
	free(list->head->prev);
	list->head->prev = NULL;
	return ret;
}

VALUE_TYPE priority_list_front(Prioritylist *list)
{
	assert(priority_list_size(list) == list->size);
	assert(list->size != 0);

	return list->head->value; 
}

int priority_list_empty(Prioritylist *list)
{
	assert(priority_list_size(list) == list->size);

	return list->size == 0;
}

static int priority_list_size(Prioritylist *list)
{
	if(list->head == NULL && list->tail == NULL)
	{
		return 0;
	}
	else
	{
		int counter = 0;
		PriorityListNode* ptr;
		for(ptr=list->head; ptr=ptr->next; ptr != NULL)
		{
			counter++;
		}
		return counter;
	}
}