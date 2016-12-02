#ifndef HARDWARE_FIFO_H_
#define HARDWARE_FIFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "circular_buffer.h"
#include "list.h"

typedef void* TYPE;
typedef struct HardwareFifo HardwareFifo;

struct HardwareFifo{
	uint size;
	uint num_fifos;
	uint num_buffers;
	uint blk_size;
	List** fifos;
	List* buffers;
};

HardwareFifo* fifo_constructor(uint size, uint num_fifos, uint num_buffers, uint blk_size);
uint fifo_push(uint id, TYPE value, HardwareFifo* fifo);
uint fifo_pop(uint id, HardwareFifo* fifo);
TYPE fifo_top(uint id, HardwareFifo* fifo);

#endif
