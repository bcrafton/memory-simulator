#include "../header/hash_map.h"

static int hash(KEY_TYPE key, HashMap* map);
static void set(int index, VALUE_TYPE value, HashMap *map);
static VALUE_TYPE get(int index, HashMap *map);

HashMap* hash_map_constructor(int capacity, int (*hashmap_hash_function)(void*) ){
	HashMap *map = malloc(sizeof(HashMap));
	map->capacity = capacity;
	map->array = malloc(map->capacity * sizeof(VALUE_TYPE));
	map->hashmap_hash_function = hashmap_hash_function;
	return map;
}

VALUE_TYPE hash_map_get(KEY_TYPE key, HashMap* map){
	int index = hash(key, map) % map->capacity;
	return get(index, map);
}

void hash_map_add(KEY_TYPE key, VALUE_TYPE value, HashMap* map){
	int index = hash(key, map) % map->capacity;
	set(index, value, map);
}

int hash_map_contains(KEY_TYPE key, HashMap* map){
	int index = hash(key, map) % map->capacity;
	return get(index, map) != NULL;
}

int hash_map_size(HashMap* map){
	int element_count = 0;
	int counter;
	for(counter = 0; counter < map->capacity; counter++){
		if(get(counter, map) != NULL){
			element_count++;
		}
	}
	return element_count;
}

static VALUE_TYPE get(int index, HashMap *map){
	return map->array[index];
}
static void set(int index, VALUE_TYPE value, HashMap *map){
	map->array[index] = value;
}

static int hash(KEY_TYPE key, HashMap* map){
	return (*(map->hashmap_hash_function))(key);
}

