#ifndef PRIORITYQUEUE_H_
#define PRIORITYQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <List.h>

typedef void* KEY_TYPE;
typedef void* VALUE_TYPE;
typedef struct PriorityList PriorityList;
typedef struct PriorityListNode PriorityListNode;

struct PriorityList{
	unsigned int size;
	PriorityListNode* head;
	PriorityListNode* tail;
	int (*compare_function)(void*, void*);
};

struct PriorityListNode{
	KEY_TYPE key;
	VALUE_TYPE value;
	PriorityListNode* next;
	PriorityListNode* prev;
};

Prioritylist* priority_list_constructor( int (*priority_list_compare_function)(void*, void*) );
void priority_list_push(KEY_TYPE key, VALUE_TYPE value, Prioritylist *list);
VALUE_TYPE priority_list_pop(Prioritylist *list);
VALUE_TYPE priority_list_front(Prioritylist *list);
int priority_list_empty(Prioritylist *list);

#endif
