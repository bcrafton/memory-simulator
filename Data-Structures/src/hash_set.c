#include "../header/hash_set.h"

static int hash(SET_TYPE value, HashSet* set);
static SET_TYPE get(int index, HashSet *set);
static void setIndex(int index, SET_TYPE value, HashSet *set);

HashSet* hash_set_constructor(int capacity, int (*hashset_hash_function)(void*) ){
	HashSet *set = malloc(sizeof(HashSet));
	set->capacity = capacity;
	set->array = malloc(set->capacity * sizeof(SET_TYPE));
	set->hashset_hash_function = hashset_hash_function;
	return set;
}

void hash_set_add(SET_TYPE value, HashSet* set){
	int index = hash(value, set) % set->capacity;
	setIndex(index, value, set);
}

int hash_set_contains(SET_TYPE value, HashSet* set){
	int index = hash(value, set) % set->capacity;
	return get(index, set) != NULL;
}
static SET_TYPE get(int index, HashSet *set){
	return set->array[index];
}
static void setIndex(int index, SET_TYPE value, HashSet *set){
	set->array[index] = value;
}
static int hash(SET_TYPE value, HashSet* set){
	return (*(set->hashset_hash_function))(value);
}
