#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct Queue Queue;
typedef void* QUEUE_TYPE;

struct Queue{
	List *list;
};

Queue* queue_constructor();
void queue_push(QUEUE_TYPE value, Queue *queue);
QUEUE_TYPE queue_pop(Queue *queue);
int queueIsEmpty(Queue *queue);
QUEUE_TYPE queue_front(Queue *queue);

#endif