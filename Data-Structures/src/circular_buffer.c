#include "../header/circular_buffer.h"

CircularBuffer* buffer_constructor(uint size)
{
	CircularBuffer* buf = (CircularBuffer*)malloc(sizeof(CircularBuffer) + size*sizeof(TYPE));
	buf->size = size;
	buf->rd = 0;
	buf->wr = 0;
	return buf;
}

uint buffer_push(TYPE value, CircularBuffer* buf)
{
	if(buffer_full(buf))
	{
		return 0;
	}

	buf->pool[buf->wr] = value;
	buf->wr++;
	if(buf->wr == buf->size)
	{
		buf->wr = 0;
	}

	return 1;
}

uint buffer_pop(CircularBuffer* buf)
{
	if(buffer_empty(buf))
	{
		return 0;
	}

	if(buf->rd == buf->size - 1)
	{
		buf->rd = 0;
	}

	else
	{
		buf->rd++;
	}

	return 1;
}

TYPE buffer_top(CircularBuffer* buf)
{
	if(buffer_empty(buf))
	{
		return 0;
	}
	else
	{
		return buf->pool[buf->rd];
	}
}

uint buffer_empty(CircularBuffer* buf)
{
	// if there are no elements in the buffer then its empty
	return buffer_size(buf) == 0;
}

uint buffer_size(CircularBuffer* buf)
{
	if(buf->wr >= buf->rd)
	{
		return buf->wr - buf->rd;
	}
	else
	{
		return buf->size - buf->rd + buf->wr;
	}
}

uint buffer_full(CircularBuffer* buf)
{
	// if the number of elements in the buffer is equal to max size
	// then its full
	return buffer_size(buf) == buf->size;
}
