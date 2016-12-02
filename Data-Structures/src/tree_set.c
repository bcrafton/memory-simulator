#include "../header/tree_set.h"

void tree_set_put(SET_TYPE value, TreeSet* set){
	rbtree_put(value, value, set->tree);
}

int tree_set_contains(SET_TYPE value, TreeSet* set){
	return rbtree_contains(value, set->tree);
}

TreeSet* tree_set_constructor( int (*treeset_compare_function)(void*, void*) ){
	TreeSet *set = malloc(sizeof(TreeSet));
	set->tree = rbtree_constructor(treeset_compare_function);
	return set;
}
