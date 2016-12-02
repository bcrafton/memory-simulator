#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/iterator.h"
#include <assert.h>

int main(){
	Iterator* iterator = iterator_constructor();
	iterator_push("Brian", iterator);
	iterator_push("Rex", iterator);
	iterator_push("Julia", iterator);

	assert(strcmp((char*)iterator_next(iterator), "Brian") == 0);
	assert(strcmp((char*)iterator_next(iterator), "Rex") == 0);
	assert(strcmp((char*)iterator_next(iterator), "Julia") == 0);

	iterator_push("Brian", iterator);
	iterator_push("Rex", iterator);
	iterator_push("Julia", iterator);

	while(iterator_hasNext(iterator)){
		printf("%s\n", (char*)iterator_next(iterator));
	}

	printf("test complete\n");
}
