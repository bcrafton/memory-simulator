#ifndef PRIORITYQUEUE_H_
#define PRIORITYQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

typedef void* KEY_TYPE;
typedef void* VALUE_TYPE;
typedef struct PriorityQueue PriorityQueue;

struct PriorityQueue{
	Heap *heap;
};

PriorityQueue* priorityqueue_constructor( int (*priority_queue_compare_function)(void*, void*) );
void priorityqueue_push(KEY_TYPE key, VALUE_TYPE value, PriorityQueue *queue);
VALUE_TYPE priorityqueue_pop(PriorityQueue *queue);
VALUE_TYPE priorityqueue_front(PriorityQueue *queue);
int priorityqueueIsEmpty(PriorityQueue *queue);
void priorityqueueSetKey(KEY_TYPE key, VALUE_TYPE value, PriorityQueue* queue);

#endif
