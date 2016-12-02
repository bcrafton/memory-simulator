#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct HashMap HashMap;
typedef void* VALUE_TYPE;
typedef void* KEY_TYPE;

struct HashMap{
	VALUE_TYPE *array;
	int capacity;
	int (*hashmap_hash_function)(void*);
};

void hash_map_add(KEY_TYPE key, VALUE_TYPE value, HashMap* map);
int hash_map_contains(KEY_TYPE key, HashMap* map);
VALUE_TYPE hash_map_get(KEY_TYPE key, HashMap* map);
HashMap* hash_map_constructor(int capacity, int (*hashmap_hash_function)(void*) );
int hash_map_size(HashMap* map);

#endif