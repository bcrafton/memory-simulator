#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct Stack Stack;
typedef void* STACK_TYPE;

struct Stack{
	List *list;
};

Stack* stack_constructor();
void stack_push(STACK_TYPE value, Stack *stack);
STACK_TYPE stack_pop(Stack *stack);
int stackIsEmpty(Stack *stack);
STACK_TYPE stack_top(Stack *stack);

#endif