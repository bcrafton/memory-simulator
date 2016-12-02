#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/tree_map.h"

int cstring_compare(void *o1, void *o2){
	return strcmp((char*)o1, (char*)o2);
}
int int_compare(void *o1, void *o2){
	return *((int*)o1) - *((int*)o2);
}
int* toPointer(int value){
	int* temp = malloc(sizeof(int));
	*temp = value;
	return temp;
}

int main(){
	char* a = "Brian";
	char* b = "Julia";
	char* c = "Rex";

	TreeMap* map = tree_map_constructor(&cstring_compare);
	tree_map_put(a, "Crafton", map);
	tree_map_put(b, "O'brien", map);
	tree_map_put(c, "Hoadly", map);

	TreeMap* map1 = tree_map_constructor(&int_compare);
	tree_map_put(toPointer(10), toPointer(13), map1);
	tree_map_put(toPointer(11), toPointer(14), map1);
	tree_map_put(toPointer(12), toPointer(15), map1);

	printf("%d\n", tree_map_contains(a, map));
	printf("%d\n", tree_map_contains("Julia", map));
	printf("%d\n", tree_map_contains("hello", map));

	printf("%s\n", (char*)tree_map_get(a, map));
	printf("%s\n", (char*)tree_map_get("Julia", map));

	printf("%d\n", tree_map_contains(toPointer(10), map1));
	printf("%d\n", tree_map_contains(toPointer(11), map1));
	printf("%d\n", tree_map_contains(toPointer(144), map1));

	printf("%d\n", *((int*)tree_map_get(toPointer(10), map1)));
	printf("%d\n", *((int*)tree_map_get(toPointer(11), map1)));
}
