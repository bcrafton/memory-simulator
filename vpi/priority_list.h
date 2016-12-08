#ifndef PriorityList_H_
#define PriorityList_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

PriorityList* priority_list_constructor( int (*priority_list_compare_function)(void*, void*) );
void priority_list_push(KEY_TYPE key, VALUE_TYPE value, PriorityList *list);
VALUE_TYPE priority_list_pop(PriorityList *list);
VALUE_TYPE priority_list_front(PriorityList *list);
int priority_list_empty(PriorityList *list);

#endif
