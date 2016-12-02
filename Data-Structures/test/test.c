#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/binarytree.h"
#include "../header/rbtree.h"
#include <assert.h>

void int_print(void *s){
	printf("%d", *((int*)s));
}
int int_compare(void *o1, void *o2){
	return *((int*)o1) - *((int*)o2);
}
int* toPointer(int value){
	int* temp = malloc(sizeof(int));
	*temp = value;
	return temp;
}

int main(){
	printf("starting binary tree test\n");

	Binarytree* tree = binarytree_constructor_print(&int_print, &int_compare);
	int counter;
	for(counter = 0; counter < 20000; counter++){
		binarytree_add(toPointer(counter), toPointer(counter), tree);
	}
	for(counter = 0; counter < 20000; counter++){
		binarytree_contains(toPointer(counter), tree);
	}

	printf("binarytree test complete\n");
	printf("starting red black tree test\n");

	RBTree* rbtree = rbtree_constructor_print(&int_print, &int_compare);
	for(counter = 0; counter < 100000; counter++){
		rbtree_add(toPointer(counter), toPointer(counter), rbtree);
	}
	for(counter = 0; counter < 100000; counter++){
		rbtree_contains(toPointer(counter), rbtree);
	}
	printf("red black tree test complete\n");
}
