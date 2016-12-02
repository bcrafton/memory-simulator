#include "../header/heap.h"

static int NULL_INDEX = -1; 
static int compare(HeapNode* v1, HeapNode* v2, Heap *heap);
static HeapNode* heap_get(int current_index, Heap* heap);
static int print(void* o, Heap* heap);

HeapNode* heap_node_constructor(KEY_TYPE key, VALUE_TYPE value){
	HeapNode* node = malloc(sizeof(HeapNode));
	node->key = key;
	node->value = value;
	return node;
}

Heap* heap_constructor_print( void (*heap_print_function)(void*), int (*heap_compare_function)(void*, void*) ){
	Heap* heap = malloc(sizeof(Heap));
	heap->vector = vector_constructor();
	heap->size = 0;
	heap->heap_compare_function = heap_compare_function;
	heap->heap_print_function = heap_print_function;
	return heap;
}

Heap* heap_constructor( int (*heap_compare_function)(void*, void*) ){
	Heap* heap = malloc(sizeof(Heap));
	heap->vector = vector_constructor();
	heap->size = 0;
	heap->heap_compare_function = heap_compare_function;
	heap->heap_print_function = NULL;
	return heap;
}

int heap_parent(int current_index){
	return current_index/2;
}
int heap_right(int current_index){
	return 2*(current_index+1);
}
int heap_left(int current_index){
	return 2*current_index+1;
}


void heap_maxHeapify(int current_index, Heap* heap){
	int left_child_index;
	int right_child_index;
	int largest_index = NULL_INDEX;
	Vector *vector = heap->vector;
	int heapSize = heap->size;

	while(largest_index != current_index){
		left_child_index = heap_left(current_index);
		right_child_index = heap_right(current_index);

		HeapNode* current = vector_get(current_index, vector);
		HeapNode* left_child = vector_get(left_child_index, vector);
		HeapNode* right_child = vector_get(right_child_index, vector);

		if(left_child_index <= heapSize-1 && compare(left_child, current, heap) > 0 ) {
			largest_index = left_child_index;
		} else {
			largest_index = current_index;
		}

		HeapNode* largest = vector_get(largest_index, vector);

		if(right_child_index <= heapSize-1 && compare(right_child, largest, heap) > 0 ) {
			largest_index = right_child_index;
		}

		if(largest_index != current_index) {
			vector_swap(current_index, largest_index, vector);
			current_index = largest_index;
			largest_index = NULL_INDEX;
		}
	}
}

void heap_add(KEY_TYPE key, VALUE_TYPE value, Heap *heap){
	HeapNode* node = heap_node_constructor(key, value);
	vector_add(node, heap->vector);
	heap->size++;
}

void heap_buildMaxHeap(Heap* heap){
	int index_counter;
	for(index_counter = (heap->size)/2-1; index_counter >= 0; index_counter--)
	{
		heap_maxHeapify(index_counter, heap);
	}
}

void heap_minHeapify(int current_index, Heap *heap){
	int left_child_index;
	int right_child_index;
	int smallest_index = NULL_INDEX;
	Vector *vector = heap->vector;
	int heapSize = heap->size;

	while(smallest_index != current_index){
		left_child_index = heap_left(current_index);
		right_child_index = heap_right(current_index);

		HeapNode* current = vector_get(current_index, vector);
		HeapNode* left_child = vector_get(left_child_index, vector);
		HeapNode* right_child = vector_get(right_child_index, vector);

		if(left_child_index <= heapSize-1 && compare(left_child, current, heap) < 0 ) {
			smallest_index = left_child_index;
		} else {
			smallest_index = current_index;
		}

		HeapNode* smallest = vector_get(smallest_index, vector);

		if(right_child_index <= heapSize-1 && compare(right_child, smallest, heap) < 0 ) {
			smallest_index = right_child_index;
		}

		if(smallest_index != current_index) {
			vector_swap(current_index, smallest_index, vector);
			current_index = smallest_index;
			smallest_index = NULL_INDEX;
		}
	}
}

void heap_buildMinHeap(Heap *heap){
	int index_counter;
	for(index_counter = (heap->size)/2-1; index_counter >= 0; index_counter--)
	{
		heap_minHeapify(index_counter, heap);
	}
}

void heap_sort_inc(Heap *heap){
	heap_buildMaxHeap(heap);

	int counter;
	int heapSize = heap->size;

	for(counter = heapSize - 1; counter >= 1; counter--){
		vector_swap(0, counter, heap->vector);
		heap->size--;
		heap_maxHeapify(0, heap);
	}
	heap->size = heapSize;
}

void heap_sort_dec(Heap *heap){
	heap_buildMinHeap(heap);

	int counter;
	int heapSize = heap->size;

	for(counter = heapSize - 1; counter >= 1; counter--){
		vector_swap(0, counter, heap->vector);
		heap->size--;
		heap_minHeapify(0, heap);
	}
	heap->size = heapSize;
}

void heap_print(Heap *heap){
	int indexCounter;
	for(indexCounter = 0; indexCounter < vector_size(heap->vector); indexCounter++){
		print(vector_get(indexCounter, heap->vector), heap);
		printf("|");
	}
	printf("[%d]\n", vector_size(heap->vector));
}

VALUE_TYPE getMaxHeapMaximum(Heap *heap){
	HeapNode* node = vector_get(0, heap->vector);
	return node->value;
}
VALUE_TYPE getMinHeapMinimum(Heap *heap){
	HeapNode* node = vector_get(0, heap->vector);
	return node->value;
}

// will use vector_set rather than remove and insert for effieciency reasons.
VALUE_TYPE extractMaxHeapMaximum(Heap *heap){
	HeapNode* top = vector_get(0, heap->vector);
	vector_set(0, heap_get(heap->size-1, heap), heap->vector);
	vector_removeIndex(heap->size-1, heap->vector);
	heap->size--;
	heap_maxHeapify(0, heap);
	return top->value;
}
// will use vector_set rather than remove and insert for effieciency reasons.
VALUE_TYPE extractMinHeapMinimum(Heap *heap){
	HeapNode* top = vector_get(0, heap->vector);
	vector_set(0, heap_get(heap->size-1, heap), heap->vector);
	vector_removeIndex(heap->size-1, heap->vector);
	heap->size--;
	heap_minHeapify(0, heap);
	return top->value;
}

void maxHeapIncreaseKey(int current_index, Heap *heap){
	while(current_index > 0 && compare(vector_get(heap_parent(current_index), heap->vector), vector_get(current_index, heap->vector), heap) < 0){
		vector_swap(current_index, heap_parent(current_index), heap->vector);
      	current_index = heap_parent(current_index);
	}
}
void minHeapDecreaseKey(int current_index, Heap *heap){
	while(current_index > 0 && compare(vector_get(heap_parent(current_index), heap->vector), vector_get(current_index, heap->vector), heap) > 0){
		vector_swap(current_index, heap_parent(current_index), heap->vector);
      	current_index = heap_parent(current_index);
	}
}

void maxHeapInsert(KEY_TYPE key ,VALUE_TYPE value, Heap *heap){
	heap_add(key, value, heap);
	maxHeapIncreaseKey(heap->size-1, heap);
}
void minHeapInsert(KEY_TYPE key ,VALUE_TYPE value, Heap *heap){
	heap_add(key, value, heap);
	minHeapDecreaseKey(heap->size-1, heap);
}

int heap_getIndex(VALUE_TYPE value, Heap *heap){
	int counter;
	for(counter = 0; counter < vector_size(heap->vector); counter++){
		HeapNode* node = vector_get(counter, heap->vector);
		if(value == node->value){
			return counter;
		}
	}
	// did not find the index.
	//assert(0);
	return -1;
}

void heap_setIndex(int index, KEY_TYPE key, VALUE_TYPE value, Heap *heap){
	if(index >= heap->size){return;}
	HeapNode* node = heap_node_constructor(key, value);
	vector_set(index, node, heap->vector);
}

static HeapNode* heap_get(int current_index, Heap* heap){
	HeapNode* node = vector_get(current_index, heap->vector);
	return node;
}

static int compare(HeapNode* v1, HeapNode* v2, Heap *heap){
	KEY_TYPE key1 = v1->key;
	KEY_TYPE key2 = v2->key;
	return (*(heap->heap_compare_function))(key1, key2);
}

static int print(void* o, Heap* heap){
	HeapNode* node = o;
	VALUE_TYPE value = node->value;
	(*(heap->heap_print_function))(value);
}

