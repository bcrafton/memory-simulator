#include "../header/queue.h"

Queue* queue_constructor(){
	Queue *queue = malloc(sizeof(Queue));
	queue->list = list_constructor();
	return queue;
}
void queue_push(QUEUE_TYPE value, Queue *queue){
	list_append(value, queue->list);
}
QUEUE_TYPE queue_pop(Queue *queue){
	QUEUE_TYPE front = queue_front(queue);
	list_remove(0, queue->list);
	return front;
}
QUEUE_TYPE queue_front(Queue *queue){
	return list_get(0, queue->list);
}
int queueIsEmpty(Queue *queue){
	return queue->list->size == 0;
}
