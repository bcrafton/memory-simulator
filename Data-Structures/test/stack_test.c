#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/stack.h"
#include <assert.h>

int main(){

	char* a1 = "Brian";
	char* b1 = "Julia";
	char* c1 = "Rex";

	Stack* stack = stack_constructor();
	stack_push(a1, stack);
	stack_push(b1, stack);
	stack_push(c1, stack);
	
	assert(strcmp((char*)stack_pop(stack), c1) == 0);
	assert(strcmp((char*)stack_pop(stack), b1) == 0);
	assert(strcmp((char*)stack_pop(stack), a1) == 0);

	printf("test complete\n");
}
