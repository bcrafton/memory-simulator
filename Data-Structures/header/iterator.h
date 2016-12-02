#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct Iterator Iterator;
typedef void* ITR_TYPE;

struct Iterator{
	List *list;
};

Iterator* iterator_constructor();
void iterator_push(ITR_TYPE value, Iterator *iterator);
ITR_TYPE iterator_next(Iterator *iterator);
int iterator_hasNext(Iterator *iterator);

#endif