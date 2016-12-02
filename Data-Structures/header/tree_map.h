#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

typedef struct TreeMap TreeMap;
typedef void* KEY_TYPE;
typedef void* VALUE_TYPE;

struct TreeMap{
	RBTree *tree;
};


void tree_map_put(KEY_TYPE key, VALUE_TYPE value, TreeMap* map);
int tree_map_contains(KEY_TYPE key, TreeMap* map);
VALUE_TYPE tree_map_get(KEY_TYPE key, TreeMap* map);
TreeMap* tree_map_constructor( int (*treemap_compare_function)(void*, void*) );
int tree_map_size(TreeMap* map);

#endif