
#include "memory_simulator.h"

static unsigned short memory[MEMORY_SIZE];

static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;

void mem_init()
{
    rd_rqst_queue = priorityqueue_constructor(&time_compare);
    wr_rqst_queue = priorityqueue_constructor(&time_compare);
}

void mem_rd_rqst(WORD start_address, TIME current_time)
{
    mem_rd_rqst_t* rqst = (mem_rd_rqst_t*) malloc(sizeof(mem_rd_rqst_t));
    rqst->start_address = start_address;
    rqst->time = current_time;

    priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);
}

void mem_wr_rqst(WORD* data, WORD start_address, TIME current_time)
{
    mem_wr_rqst_t* rqst = (mem_wr_rqst_t*) malloc(sizeof(mem_wr_rqst_t));
    rqst->start_address = start_address;
    rqst->time = current_time;

    memcpy(rqst->data, data, WORDS_PER_CACHE_LINE * sizeof(WORD));

    priorityqueue_push(&rqst->time, rqst, wr_rqst_queue);
}

mem_rd_ret_t* mem_rd_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(rd_rqst_queue))
    {
        return NULL;
    }
    mem_rd_rqst_t* rqst = (mem_rd_rqst_t*) priorityqueue_front(rd_rqst_queue);
    if(rqst->time <= current_time)
    {
        priorityqueue_pop(rd_rqst_queue);

        mem_rd_ret_t* ret = (mem_rd_ret_t*) malloc(sizeof(mem_rd_ret_t));
        memcpy(ret->data, memory + rqst->start_address * sizeof(WORD), WORDS_PER_CACHE_LINE * sizeof(WORD));
        ret->start_address = rqst->start_address;
        return ret;
    }
    return NULL;
}

mem_wr_ret_t* mem_wr_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(wr_rqst_queue))
    {
        return NULL;
    }
    mem_wr_rqst_t* rqst = (mem_wr_rqst_t*) priorityqueue_front(wr_rqst_queue);
    if(rqst->time <= current_time)
    {
        priorityqueue_pop(wr_rqst_queue);

        memcpy(memory + rqst->start_address * sizeof(WORD), rqst->data, WORDS_PER_CACHE_LINE * sizeof(WORD));

        mem_wr_ret_t* ret = (mem_wr_ret_t*) malloc(sizeof(mem_rd_ret_t));
        ret->start_address = rqst->start_address;
        return ret;
    }
    return NULL;
}













