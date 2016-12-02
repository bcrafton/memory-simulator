#include "../header/priority_queue.h"

PriorityQueue* priorityqueue_constructor( int (*priority_queue_compare_function)(void*, void*) ){
	PriorityQueue *queue = malloc(sizeof(PriorityQueue));
	queue->heap = heap_constructor(priority_queue_compare_function);
	return queue;
}
void priorityqueue_push(KEY_TYPE key, VALUE_TYPE value, PriorityQueue *queue){
	minHeapInsert(key, value, queue->heap);
}
VALUE_TYPE priorityqueue_pop(PriorityQueue *queue){
	return extractMinHeapMinimum(queue->heap);
}
VALUE_TYPE priorityqueue_front(PriorityQueue *queue){
	return getMinHeapMinimum(queue->heap);
}
int priorityqueueIsEmpty(PriorityQueue *queue){
	return queue->heap->size == 0;
}
// this is basically gonna tell the heap to check its key in the index location. 
// cud actually change the key but since pointer its all good.
// values are unique, not keys so w.e 

// this is rather ineffiecient.
void priorityqueueSetKey(KEY_TYPE key, VALUE_TYPE value, PriorityQueue* queue){
	int index = heap_getIndex(value, queue->heap);
	if(index != -1){
		heap_setIndex(index, key, value, queue->heap);
		minHeapDecreaseKey(index, queue->heap);
	}
}
