#include "../header/tree_map.h"

void tree_map_put(KEY_TYPE key, VALUE_TYPE value, TreeMap* map){
	rbtree_put(key, value, map->tree);
}
int tree_map_contains(KEY_TYPE key, TreeMap* map){
	return rbtree_contains(key, map->tree);
}
VALUE_TYPE tree_map_get(KEY_TYPE key, TreeMap* map){
	return rbtree_search(key, map->tree);
}
TreeMap* tree_map_constructor( int (*treemap_compare_function)(void*, void*) ){
	TreeMap *map = malloc(sizeof(TreeMap));
	map->tree = rbtree_constructor(treemap_compare_function);
	return map;
}
int tree_map_size(TreeMap* map){
	return map->tree->size;
}
