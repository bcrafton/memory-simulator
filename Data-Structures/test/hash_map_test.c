#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/hash_map.h"

void int_print(void *s){
	printf("%d", *((int*)s));
}
void cstring_print(void *s){
	printf("%s", (char*)s);
}
int int_compare(void *o1, void *o2){
	if(o1 == NULL && o2 == NULL){
		return 0;
	}
	if(o1 == NULL){
		return -1;
	}
	if(o2 == NULL){
		return 1;
	}
	return *((int*)o1) - *((int*)o2);
}
int cstring_compare(void *o1, void *o2){
	if(o1 == NULL && o2 == NULL){
		return 0;
	}
	if(o1 == NULL){
		return -1;
	}
	if(o2 == NULL){
		return 1;
	}
	return strcmp((char*)o1, (char*)o2);
}
int int_hash(void *v){
	return *((int*)v);
}
int cstring_hash(void *v){
	int hash = 0;
	int indexCounter;
	char* s = (char*)v;

	for(indexCounter = 0; indexCounter < strlen(s) || indexCounter < 4; indexCounter++){
		if(indexCounter != 0){
			hash = hash << 8;
		}
		hash = hash | s[indexCounter];
	}
	return hash;
}

int main(){
	char* a = "Brian";
	char* b = "Julia";
	char* c = "Rex";

	HashMap* map = hash_map_constructor(100, &cstring_hash);
	hash_map_add(a, "Crafton", map);
	hash_map_add(b, "O'brien", map);
	hash_map_add(c, "Hoadly", map);

	printf("%d\n", hash_map_contains(a, map));
	printf("%d\n", hash_map_contains("Julia", map));
	printf("%d\n", hash_map_contains("hello", map));

	printf("%s\n", (char*)hash_map_get(a, map));
	printf("%s\n", (char*)hash_map_get("Julia", map));
}
