#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"

typedef void* TYPE;
typedef struct CircularBuffer CircularBuffer;

struct CircularBuffer{
	uint size;
	uint rd;
	uint wr;
	TYPE pool[];
};

CircularBuffer* buffer_constructor(uint size);
uint buffer_push(TYPE value, CircularBuffer* buf);
uint buffer_pop(CircularBuffer* buf);
TYPE buffer_top(CircularBuffer* buf);
uint buffer_empty(CircularBuffer* buf);
uint buffer_size(CircularBuffer* buf);
uint buffer_full(CircularBuffer* buf);

#endif
