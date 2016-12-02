#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/queue.h"
#include <assert.h>

int main(){

	char* a1 = "Brian";
	char* b1 = "Julia";
	char* c1 = "Rex";

	Queue* queue = queue_constructor();
	queue_push(a1, queue);
	queue_push(b1, queue);
	queue_push(c1, queue);
	
	assert(strcmp((char*)queue_pop(queue), a1) == 0);
	assert(strcmp((char*)queue_pop(queue), b1) == 0);
	assert(strcmp((char*)queue_pop(queue), c1) == 0);
	
	printf("test complete\n");
}
