#include "priority_list.h"
#include <assert.h>

typedef struct cache_rd_rqst{
    unsigned short address;
    unsigned long time;
} cache_rd_rqst_t;

int time_compare(void *o1, void *o2){
	return (int) *((unsigned long*)o1) - *((unsigned long*)o2);
}

void print_list(PriorityList* list)
{
    PriorityListNode* ptr;
    for(ptr=list->head; ptr != NULL; ptr=ptr->next)
    {
        printf("%u\n", *(unsigned short*)ptr->value);
    }
    for(ptr=list->tail; ptr != NULL; ptr=ptr->prev)
    {
        printf("%u\n", *(unsigned short*)ptr->value);
    }
}

int main()
{
    cache_rd_rqst_t a = {1, 1};
    cache_rd_rqst_t b = {2, 2};
    cache_rd_rqst_t c = {3, 3};
    cache_rd_rqst_t d = {4, 4};
    cache_rd_rqst_t e = {5, 5};
    cache_rd_rqst_t f = {6, 6};
    cache_rd_rqst_t g = {7, 7};

    PriorityList* list = priority_list_constructor(&time_compare);
    priority_list_push(&c.time, &c, list);
    priority_list_push(&b.time, &b, list);
    
    priority_list_push(&e.time, &e, list);
    // 2 3 5
    printf("%lu\n", *(unsigned long*)priority_list_pop(list));
    // 3 5
    priority_list_push(&a.time, &a, list);
    // 1 3 5
    priority_list_push(&f.time, &f, list);
    // 1 3 5 6
    priority_list_push(&d.time, &d, list);
    // 1 3 4 5 6
    priority_list_push(&g.time, &g, list);
    // 1 3 4 5 6

    printf("%lu\n", *(unsigned long*)priority_list_pop(list));
    print_list(list);


    printf("%lu\n", *(unsigned long*)priority_list_pop(list));
    /*
    printf("%lu\n", *(unsigned long*)priority_list_pop(list));
    printf("%lu\n", *(unsigned long*)priority_list_pop(list));
    printf("%lu\n", *(unsigned long*)priority_list_pop(list));
    printf("%lu\n", *(unsigned long*)priority_list_pop(list));
    */
}
