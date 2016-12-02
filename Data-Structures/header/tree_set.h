#ifndef TREESET_H_
#define TREESET_H_

#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

typedef struct TreeSet TreeSet;
typedef void* SET_TYPE;

struct TreeSet{
	RBTree *tree;
};

void tree_set_put(SET_TYPE value, TreeSet* set);
int tree_set_contains(SET_TYPE value, TreeSet* set);
TreeSet* tree_set_constructor( int (*treeset_compare_function)(void*, void*) );


#endif