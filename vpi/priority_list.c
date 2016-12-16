
#include "priority_list.h"

static int priority_list_size(PriorityList *list);

PriorityList* priority_list_constructor( int (*compare_function)(void*, void*) )
{
	PriorityList* priority_list = (PriorityList*) malloc(sizeof(PriorityList));
	priority_list->compare_function = compare_function;
	priority_list->head = NULL;
	priority_list->tail= NULL;
	priority_list->size = 0;
	return priority_list;
}

PriorityListNode* priority_list_node_constructor(KEY_TYPE key, VALUE_TYPE value)
{
	PriorityListNode* new_node = (PriorityListNode*) malloc (sizeof(PriorityListNode));
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

// I am skeptical of the correctness of this function
void priority_list_push(KEY_TYPE key, VALUE_TYPE value, PriorityList *list)
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
		for(ptr=list->head; ptr != NULL; ptr=ptr->next)
		{
			// 3 cases, insert head, insert tail, insert middle
			if( ((*(list->compare_function))(new_node->key, ptr->key)) < 0 )
			{
				if(ptr->prev == NULL)
                {
                    ptr->prev = new_node;
                    new_node->next = ptr;
                    list->head = new_node;
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
				list->tail = new_node;
				break;
			}
		}
	}
}

VALUE_TYPE priority_list_pop(PriorityList *list)
{
	assert(priority_list_size(list) == list->size);
	assert(list->size != 0);
	
	VALUE_TYPE ret = list->head->value;
	if(list->head == list->tail)
	{
		assert(list->size == 1);
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		assert(list->size > 1);
		list->head = list->head->next;
		free(list->head->prev);
		list->head->prev = NULL;
	}
	list->size--;
	return ret;
}

VALUE_TYPE priority_list_front(PriorityList *list)
{
	assert(priority_list_size(list) == list->size);
	assert(list->size != 0);

	return list->head->value; 
}

int priority_list_empty(PriorityList *list)
{
	assert(priority_list_size(list) == list->size);

	return list->size == 0;
}

static int priority_list_size(PriorityList *list)
{
	if(list->head == NULL && list->tail == NULL)
	{
		return 0;
	}
	else
	{
		int counter = 0;
		PriorityListNode* ptr;
		for(ptr=list->head; ptr != NULL; ptr=ptr->next)
		{
			counter++;
		}
		return counter;
	}
}