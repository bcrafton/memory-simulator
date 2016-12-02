#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/hash_set.h"

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

	HashSet* set = hash_set_constructor(100, &cstring_hash);
	hash_set_add(a, set);
	hash_set_add(b, set);
	hash_set_add(c, set);

	printf("%d\n", hash_set_contains(a, set));
	printf("%d\n", hash_set_contains("Julia", set));
	printf("%d\n", hash_set_contains("hello", set));
}
