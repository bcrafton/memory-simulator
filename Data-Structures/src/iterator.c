#include "../header/iterator.h"

Iterator* iterator_constructor(){
	Iterator *iterator = malloc(sizeof(Iterator));
	iterator->list = list_constructor();
	return iterator;
}
void iterator_push(ITR_TYPE value, Iterator *iterator){
	list_append(value, iterator->list);
}
ITR_TYPE iterator_next(Iterator *iterator){
	ITR_TYPE next = list_get(0, iterator->list);
	list_remove(0, iterator->list);
	return next;
}
int iterator_hasNext(Iterator *iterator){
	return iterator->list->size != 0;
}
