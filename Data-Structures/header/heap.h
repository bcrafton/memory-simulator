#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <assert.h>

typedef struct Heap Heap;
typedef struct HeapNode HeapNode;
typedef void* KEY_TYPE;
typedef void* VALUE_TYPE;

struct Heap{
	Vector *vector;
	int size;
	int (*heap_compare_function)(void*, void*);
	void (*heap_print_function)(void*);
};

struct HeapNode{
	KEY_TYPE key;
	VALUE_TYPE value;
};

HeapNode* heap_node_constructor(KEY_TYPE key, VALUE_TYPE value);
Heap* heap_constructor_print( void (*heap_print_function)(void*), int (*heap_compare_function)(void*, void*) );
Heap* heap_constructor( int (*heap_compare_function)(void*, void*) );
int heap_parent(int current_index);
int heap_right(int current_index);
int heap_left(int current_index);

//static HeapNode* heap_get(int current_index, Heap* heap);

void heap_setIndex(int index, KEY_TYPE key, VALUE_TYPE value, Heap *heap);
int heap_getIndex(VALUE_TYPE value, Heap *heap);
void heap_maxHeapify(int current_index, Heap* heap);
void heap_buildMaxHeap(Heap* heap);
void heap_add(KEY_TYPE key, VALUE_TYPE value, Heap *heap);
void heap_print(Heap *heap);

void heap_minHeapify(int current_index, Heap *heap);
void heap_buildMinHeap(Heap *heap);

void heap_sort_inc(Heap *heap);
void heap_sort_dec(Heap *heap);

VALUE_TYPE getMaxHeapMaximum(Heap *heap);
VALUE_TYPE getMinHeapMinimum(Heap *heap);

VALUE_TYPE extractMaxHeapMaximum(Heap *heap);
VALUE_TYPE extractMinHeapMinimum(Heap *heap);

void maxHeapIncreaseKey(int current_index, Heap *heap);
void minHeapDecreaseKey(int current_index, Heap *heap);

void maxHeapInsert(KEY_TYPE key, VALUE_TYPE value, Heap *heap);
void minHeapInsert(KEY_TYPE key, VALUE_TYPE value, Heap *heap);

#endif