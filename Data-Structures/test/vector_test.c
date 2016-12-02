#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/vector.h"
#include <assert.h>

void cstring_print(void *s){
	printf("%s", (char*)s);
}
void int_print(void *s){
	printf("%d", *((int*)s));
}
int* toPointer(int value){
	int* temp = malloc(sizeof(int));
	*temp = value;
	return temp;
}
int main(){
	char* a1 = "Brian";
	char* b1 = "Julia";
	char* c1 = "Rex";

	Vector* vector = vector_constructor_print(&cstring_print);
	vector_add(a1, vector);
	vector_add(b1, vector);
	vector_add(c1, vector);
	
	assert(strcmp((char*)vector_get(0, vector), a1) == 0);
	assert(strcmp((char*)vector_get(1, vector), b1) == 0);
	assert(strcmp((char*)vector_get(2, vector), c1) == 0);

	vector_print(vector);

	assert(vector->next == 3);
	assert(vector_size(vector) == 3);

	//vector_set(3, "Daniel", vector);
	//vector_insert(4, "Daniel", vector);
	vector_insert(0, "Ariel", vector);
	vector_insert(1, "Booth", vector);
	vector_insert(3, "Daniel", vector);
	vector_set(2, "Sam", vector);
	assert(strcmp((char*)vector_get(3, vector), "Daniel") == 0);
	assert(strcmp((char*)vector_get(2, vector), "Sam") == 0);

	vector_print(vector);
	vector_removeIndex(5, vector);
	vector_print(vector);
	vector_removeIndex(0, vector);
	vector_print(vector);
	vector_removeIndex(1, vector);
	vector_print(vector);

	vector = vector_constructor_print(&int_print);
	int counter;
	for(counter = 0; counter < 10; counter++){
		vector_insert(counter, toPointer(counter), vector);
	}
	vector_print(vector);

	vector = vector_constructor_print(&int_print);
	for(counter = 0; counter < 11; counter++){
		vector_insert(counter, toPointer(counter), vector);
	}
	vector_print(vector);

	vector = vector_constructor_print(&int_print);
	for(counter = 0; counter < 12; counter++){
		vector_insert(counter, toPointer(counter), vector);
	}
	vector_print(vector);

	vector = vector_constructor_print(&int_print);
	for(counter = 0; counter < 9; counter++){
		vector_insert(counter, toPointer(counter), vector);
	}
	vector_print(vector);

	printf("test complete\n");
}
