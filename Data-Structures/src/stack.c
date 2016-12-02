#include "../header/stack.h"

Stack* stack_constructor(){
	Stack *stack = malloc(sizeof(Stack));
	stack->list = list_constructor();
	return stack;
}

void stack_push(STACK_TYPE value, Stack *stack){
	list_prepend(value, stack->list);
}
STACK_TYPE stack_pop(Stack *stack){
	STACK_TYPE top = list_get(0, stack->list);
	list_remove(0, stack->list);
	return top;
}
STACK_TYPE stack_top(Stack *stack){
	return list_get(0, stack->list);
}
int stackIsEmpty(Stack *stack){
	return stack->list->size == 0;
}
