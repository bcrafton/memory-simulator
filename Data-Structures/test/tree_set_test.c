#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/tree_set.h"

int cstring_compare(void *o1, void *o2){
	return strcmp((char*)o1, (char*)o2);
}

int main(){
	char* a = "Brian";
	char* b = "Julia";
	char* c = "Rex";

	TreeSet* set = tree_set_constructor(&cstring_compare);
	tree_set_put(a, set);
	tree_set_put(b, set);
	tree_set_put(c, set);

	printf("%d\n", tree_set_contains(a, set));
	printf("%d\n", tree_set_contains("Julia", set));
	printf("%d\n", tree_set_contains("hello", set));
}
