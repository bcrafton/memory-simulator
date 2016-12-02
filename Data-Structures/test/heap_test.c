#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/heap.h"
#include <assert.h>
#include <time.h>

void int_print(void *s){
	printf("%d", *((int*)s));
}
void cstring_print(void *s){
	printf("%s", (char*)s);
}
int int_compare(void *o1, void *o2){
	return *((int*)o1) - *((int*)o2);
}
int cstring_compare(void *o1, void *o2){
	return strcmp((char*)o1, (char*)o2);
}
int* get_int_ptr(int value){
	int* temp = malloc(sizeof(int));
	*temp = value;
	return temp;
}

int main(){
	int a = 10;
	int b = 11;
	int c = 12;

	char* a1 = "Brian";
	char* b1 = "Julia";
	char* c1 = "Rex";

	Heap* heap = heap_constructor_print(&int_print, &int_compare);
	heap_add(&a, &a, heap);
	heap_add(&b, &b, heap);
	heap_add(&c, &c, heap);

	Heap* sheap = heap_constructor_print(&cstring_print, &cstring_compare);
	heap_add(a1, a1, sheap);
	heap_add(b1, b1, sheap);
	heap_add(c1, c1, sheap);

	heap_buildMaxHeap(heap);
	heap_print(heap);
	heap_sort_inc(heap);
	heap_print(heap);
	heap_buildMaxHeap(heap);

	heap_buildMinHeap(sheap);
	heap_print(sheap);
	heap_sort_dec(sheap);
	heap_print(sheap);
	heap_buildMinHeap(sheap);

	
	assert(int_compare(extractMaxHeapMaximum(heap), &c) == 0);
	assert(int_compare(extractMaxHeapMaximum(heap), &b) == 0);
	assert(int_compare(extractMaxHeapMaximum(heap), &a) == 0);

	assert(cstring_compare(extractMinHeapMinimum(sheap), a1) == 0);
	assert(cstring_compare(extractMinHeapMinimum(sheap), b1) == 0);
	assert(cstring_compare(extractMinHeapMinimum(sheap), c1) == 0);
	
	Heap* heap1 = heap_constructor_print(&int_print, &int_compare);
	srand(time(NULL));
	int counter = 0;
	
	for(counter = 0; counter < 100; counter++){
		int* key = get_int_ptr(rand()%100);
		heap_add(key, key, heap1);
	}
	heap_buildMaxHeap(heap1);
	heap_print(heap1);
	heap_sort_inc(heap1);
	heap_print(heap1);

	Heap* heap2 = heap_constructor_print(&int_print, &int_compare);
	srand(time(NULL));
	
	printf("\n\n");

	for(counter = 0; counter < 100; counter++){
		int* key = get_int_ptr(rand()%100);
		maxHeapInsert(key, key, heap2);
	}
	while(heap2->size > 0){
		printf("%d|", *((int*)extractMaxHeapMaximum(heap2)));
	}
	printf("\n\n");
}
