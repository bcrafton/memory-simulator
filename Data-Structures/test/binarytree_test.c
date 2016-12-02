#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/binarytree.h"
#include <assert.h>

void cstring_print(void *s){
	printf("%s", (char*)s);
}
int cstring_compare(void *o1, void *o2){
	return strcmp((char*)o1, (char*)o2);
}
int main(){
	char* a1 = "Brian";
	char* b1 = "Julia";
	char* c1 = "Rex";

	Binarytree* tree = binarytree_constructor_print(&cstring_print, &cstring_compare);
	binarytree_add(a1, a1, tree);
	binarytree_add(b1, b1, tree);
	binarytree_add(c1, c1, tree);

	printf("\n%s\n", (char*)binarytree_search(a1, tree));
	printf("\n%d\n", binarytree_contains(a1, tree));

printf("\n%d\n", binarytree_contains("hello", tree));

	binarytree_stack_print(tree);

	printf("test complete\n");
}
