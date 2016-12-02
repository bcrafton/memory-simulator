#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

typedef struct BinarytreeNode BinarytreeNode;
typedef struct Binarytree Binarytree;
typedef void* KEY_TYPE;
typedef void* VALUE_TYPE;

struct BinarytreeNode {
	KEY_TYPE key;
	VALUE_TYPE value;
	BinarytreeNode* left;
	BinarytreeNode* right;
};

struct Binarytree{
	int size;
	BinarytreeNode* root;
	void (*tree_print_function)(void*);
	int (*tree_compare_function)(void*, void*);
};

BinarytreeNode* binarytree_node_constructor(KEY_TYPE key, VALUE_TYPE value);
Binarytree* binarytree_constructor( int (*tree_compare_function)(void*, void*) );
Binarytree* binarytree_constructor_print( void (*tree_print_function)(void*), int (*tree_compare_function)(void*, void*) );
void binarytree_add(KEY_TYPE key, VALUE_TYPE value, Binarytree* tree);
int binarytree_contains(KEY_TYPE key, Binarytree* tree);
void binarytree_stack_print(Binarytree *tree);
VALUE_TYPE binarytree_search(KEY_TYPE key, Binarytree *tree);

#endif