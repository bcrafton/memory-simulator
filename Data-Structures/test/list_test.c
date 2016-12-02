#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/list.h"
#include <assert.h>

void cstring_print(void *s){
	printf("%s", (char*)s);
}

int main(){
	List* list = list_constructor_print(&cstring_print);
	list_append("Brian", list);
	list_append("Julia", list);
	list_append("Rex", list);
	
	assert(strcmp((char*)list_get(0, list), "Brian") == 0);
	assert(strcmp((char*)list_get(1, list), "Julia") == 0);
	assert(strcmp((char*)list_get(2, list), "Rex") == 0);

	list_insert(3, "Daniel", list);
	list_insert(0, "Booth", list);
	list_insert(2, "Sam", list);

	assert(strcmp((char*)list_get(0, list), "Booth") == 0);
	assert(strcmp((char*)list_get(1, list), "Brian") == 0);
	assert(strcmp((char*)list_get(2, list), "Sam") == 0);
	assert(strcmp((char*)list_get(3, list), "Julia") == 0);
	assert(strcmp((char*)list_get(4, list), "Rex") == 0);
	assert(strcmp((char*)list_get(5, list), "Daniel") == 0);

	list_remove(5, list);
	list_remove(0, list);
	list_remove(2, list);
	assert(strcmp((char*)list_get(0, list), "Brian") == 0);
	assert(strcmp((char*)list_get(1, list), "Sam") == 0);
	assert(strcmp((char*)list_get(2, list), "Rex") == 0);

	list_print(list);

	printf("test complete\n");
}
