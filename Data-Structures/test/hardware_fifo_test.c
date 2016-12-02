#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/circular_buffer.h"
#include <assert.h>


int main()
{
	CircularBuffer* buffer = buffer_constructor(100);

	int a = 10;
	int b = 20;
	int c = 30;

	buffer_push(&a, buffer);
	buffer_push(&b, buffer);
	buffer_push(&c, buffer);

	assert( *(int*)buffer_top(buffer) == 10);
	buffer_pop(buffer);
	assert( *(int*)buffer_top(buffer) == 20);
	buffer_pop(buffer);
	assert( *(int*)buffer_top(buffer) == 30);

	printf("test complete\n");
}
