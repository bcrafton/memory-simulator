
#include "memory_simulator.h"

static cache_t cache;

static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;
static List* cache_rd_miss_queue = NULL;
static List* cache_wr_miss_queue = NULL;

static BOOL in_cache(BYTE cache_line_number, BYTE tag);
static BOOL mem_rd_rqst_pending(BYTE cache_line_number, BYTE tag);
static BYTE evict_lru();

void cache_init()
{
    rd_rqst_queue = priorityqueue_constructor(&time_compare);
    wr_rqst_queue = priorityqueue_constructor(&time_compare);
    cache_rd_miss_queue = list_constructor();
    cache_wr_miss_queue = list_constructor();

    int i;
    for(i=0;i<NUM_CACHE_LINES;i++)
    {
        cache.lines[i].dirty = 0;
        cache.lines[i].next = i+1;
    }
    cache.lru = 0; 
    cache.mru = NUM_CACHE_LINES-1;
}

void cache_rd_rqst(WORD address, TIME current_time)
{
    cache_rd_rqst_t* rqst = (cache_rd_rqst_t*) malloc(sizeof(cache_rd_rqst_t));
    rqst->address = address;
    rqst->time = current_time + CACHE_READ_TIME;

    priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);
}

void cache_wr_rqst(WORD address, WORD data, TIME current_time)
{
    cache_wr_rqst_t* rqst = (cache_wr_rqst_t*) malloc(sizeof(cache_wr_rqst_t));
    rqst->address = address;
    rqst->data = data;
    rqst->time = current_time + CACHE_WRITE_TIME;

    priorityqueue_push(&rqst->time, rqst, wr_rqst_queue);
}

cache_rd_ret_t* cache_rd_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(rd_rqst_queue))
    {
        return NULL;
    }
    
    cache_rd_rqst_t* rqst = (cache_rd_rqst_t*) priorityqueue_front(rd_rqst_queue);
    
    BYTE tag = TAG(rqst->address);
    BYTE cache_line_number = CACHELINE(rqst->address);
    BYTE offset = OFFSET(rqst->address);

    if(rqst->time <= current_time)
    {
        priorityqueue_pop(rd_rqst_queue);
        if(in_cache(cache_line_number, tag))
        {
            cache_rd_ret_t* ret = (cache_rd_ret_t*) malloc(sizeof(cache_rd_ret_t));
            ret->address = rqst->address;
            ret->data = cache.lines[cache_line_number].data[offset];
            return ret;
        }
        else
        {
            list_append(rqst, cache_rd_miss_queue);
            if(!mem_rd_rqst_pending(tag, cache_line_number))
            {
                WORD start_address = (tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (cache_line_number << OFFSET_LOG2);
                mem_rd_rqst(start_address, current_time);
            }
        }
    }
    return NULL;
}

cache_wr_ret_t* cache_wr_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(wr_rqst_queue))
    {
        return NULL;
    }

    cache_wr_rqst_t* rqst = (cache_wr_rqst_t*) priorityqueue_front(wr_rqst_queue);

    BYTE tag = TAG(rqst->address);
    BYTE cache_line_number = CACHELINE(rqst->address);

    if(rqst->time <= current_time)
    {
        priorityqueue_pop(wr_rqst_queue);
        if(in_cache(cache_line_number, tag))
        {
            cache_wr_ret_t* ret = (cache_wr_ret_t*) malloc(sizeof(cache_wr_ret_t));
            ret->address = rqst->address;
            return ret;
        }
        else
        {
            list_append(rqst, cache_wr_miss_queue);
            if(!mem_rd_rqst_pending(tag, cache_line_number))
            {
                WORD start_address = (tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (cache_line_number << OFFSET_LOG2);
                mem_rd_rqst(start_address, current_time);
            }
        }
    }
    return NULL;
}

void cache_update(TIME current_time)
{
    mem_rd_ret_t* rd_ret = mem_rd_ret(current_time);
    mem_wr_ret_t* wr_ret = mem_wr_ret(current_time);

    // move the cache misses back into valid request queue
    if(rd_ret != NULL)
    {
        int i;
        for(i=0; i<cache_rd_miss_queue->size; i++)
        {
            if( in_cache(TAG(rd_ret->start_address), CACHELINE(rd_ret->start_address)) )
            {
                cache_rd_rqst_t* rqst = (cache_rd_rqst_t*) list_remove(i, cache_rd_miss_queue);
                rqst->time = current_time + CACHE_READ_TIME;
                priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);
            }
        }

        for(i=0; i<cache_wr_miss_queue->size; i++)
        {
            if( in_cache(TAG(wr_ret->start_address), CACHELINE(wr_ret->start_address)) )
            {
                cache_wr_rqst_t* rqst = (cache_wr_rqst_t*) list_remove(i, cache_wr_miss_queue);
                rqst->time = current_time + CACHE_WRITE_TIME;
                priorityqueue_push(&rqst->time, rqst, wr_rqst_queue);
            }
        }
        
        // flush lru to open up cache line for new memory
        BYTE lru = evict_lru();
        WORD start_address = (cache.lines[lru].tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (lru << OFFSET_LOG2);
        mem_wr_rqst(cache.lines[lru].data, start_address, current_time);

        // put the rd_ret into cache.
        BYTE cache_line_number = CACHELINE(rd_ret->start_address);
        memcpy(cache.lines[cache_line_number].data, rd_ret->data, NUM_CACHE_LINES * sizeof(WORD));
    }
    if(wr_ret != NULL)
    {
    }
}

static BOOL in_cache(BYTE cache_line_number, BYTE tag)
{
    return (cache.lines[cache_line_number].tag == tag && cache.lines[cache_line_number].valid==1);
}

static BOOL mem_rd_rqst_pending(BYTE cache_line_number, BYTE tag)
{
    return 0;
}

static BYTE evict_lru()
{
    BYTE evicted = cache.lru;
    cache.lru = cache.lines[cache.lru].next;
    cache.lines[cache.mru].next = evicted;
    cache.mru = evicted;
    return evicted;
}

int main()
{}

