#ifndef HASHSET_H_
#define HASHSET_H_

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

typedef struct HashSet HashSet;
typedef void* SET_TYPE;

struct HashSet{
	SET_TYPE *array;
	int capacity;
	int (*hashset_hash_function)(void*);
};

void hash_set_add(SET_TYPE value, HashSet* set);
int hash_set_contains(SET_TYPE value, HashSet* set);
HashSet* hash_set_constructor(int capacity, int (*hashset_hash_function)(void*) );


#endif