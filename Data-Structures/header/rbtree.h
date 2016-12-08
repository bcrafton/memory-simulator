#ifndef RBTREE_H_
#define RBTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

enum Color{
	Black,
	Red
};
typedef enum Color Color;

typedef struct RBTreeNode RBTreeNode;
typedef struct RBTree RBTree;
typedef void* KEY_TYPE;
typedef void* VALUE_TYPE;

struct RBTreeNode {
	KEY_TYPE key;
	VALUE_TYPE value;
	RBTreeNode* parent;
	RBTreeNode* left;
	RBTreeNode* right;
	Color color;
};

struct RBTree{
	int size;
	RBTreeNode* root;
	void (*tree_print_function)(void*);
	int (*tree_compare_function)(void*, void*);
};

RBTreeNode* rbtree_node_constructor(KEY_TYPE key, VALUE_TYPE value, RBTreeNode* parent);
RBTree* rbtree_constructor( int (*tree_compare_function)(void*, void*) );
RBTree* rbtree_constructor_print( void (*tree_print_function)(void*), int (*tree_compare_function)(void*, void*) );
void rbtree_add(KEY_TYPE key, VALUE_TYPE value, RBTree* tree);
void rbtree_put(KEY_TYPE key, VALUE_TYPE value, RBTree* tree);
int rbtree_contains(KEY_TYPE key, RBTree* tree);
void rbtree_stack_print(RBTree *tree);
VALUE_TYPE rbtree_search(KEY_TYPE key, RBTree *tree);
void rbtree_stack_inorder_print(RBTree *tree);
RBTreeNode* rbtree_get(KEY_TYPE key, RBTree* tree);
void rbtree_remove(KEY_TYPE key, RBTree* tree);



#endif
