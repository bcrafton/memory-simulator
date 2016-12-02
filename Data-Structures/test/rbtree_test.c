#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/rbtree.h"
#include <assert.h>

// rb tree remove is broken... i think

void cstring_print(void *s){
	printf("%s", (char*)s);
}
int cstring_compare(void *o1, void *o2){
	return strcmp((char*)o1, (char*)o2);
}
int main(){
	RBTree *tree = rbtree_constructor_print(&cstring_print, &cstring_compare);
	rbtree_put("Alex", "Alex", tree);
	rbtree_put("Brian", "Brian", tree);
	rbtree_put("Chris", "Chris", tree);
	rbtree_put("David", "David", tree);
	rbtree_put("Ethan", "Ethan", tree);
	rbtree_put("Fag", "Fag", tree);
	rbtree_put("George", "George", tree);
	rbtree_put("Hannibal", "Hannibal", tree);

	//printf("\n%s\n", (char*)rbtree_search("Alex", tree));
	//printf("\n%d\n", rbtree_contains("Brian", tree));

	//printf("\n%d\n", rbtree_contains("hello", tree));

	rbtree_stack_print(tree);
	rbtree_remove("Chris", tree);
	rbtree_stack_print(tree);

	printf("test complete\n");
}
